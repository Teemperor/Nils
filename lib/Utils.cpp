#include "Utils.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdio>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <mutex>
#include <wait.h>

void Utils::createDir(const std::string &Path) {
  runCmd("mkdir", {"-p", Path}).assumeGood();
}

void Utils::deleteDir(const std::string &Path) {
  runCmd("rm", {"-rf", Path}).assumeGood();
}

void Utils::deleteFile(const std::string &Path) {
  runCmd("rm", {"-f", Path}).assumeGood();
}

void Utils::copyDir(const std::string &Source, const std::string &Target) {
  deleteDir(Target);
  runCmd("cp", {"-r", Source, Target}).assumeGood();
}

void Utils::copyFile(const std::string &Source, const std::string &Target) {
  runCmd("cp", {Source, Target}).assumeGood();
}

CmdResult Utils::runCmd(const std::string &Exe,
                        const std::vector<std::string> &Args,
                        const std::string &WorkingDir) {
  int filedes[2];
  if (pipe(filedes) == -1) {
    perror("pipe");
    exit(1);
  }

  std::vector<char *> CArgs;
  CArgs.push_back(const_cast<char *>(Exe.c_str()));
  for (const std::string& Arg : Args) {
    CArgs.push_back(const_cast<char *>(Arg.c_str()));
  }
  CArgs.push_back(nullptr);

  pid_t pid = fork();
  if (pid == -1) {
    perror("fork");
    exit(1);
  } else if (pid == 0) {
    if (!WorkingDir.empty())
      chdir(WorkingDir.c_str());
    while ((dup2(filedes[1], STDOUT_FILENO) == -1) && (errno == EINTR)) {}
    close(filedes[1]);
    close(filedes[0]);

    execvp(Exe.c_str(), CArgs.data());
    perror("execvp");
    _exit(1);
  }
  close(filedes[1]);

  std::stringstream ss;
  // buffer and 0 terminate it.
  char buffer[4097];
  buffer[4096] = 0;


  while (true) {
    ssize_t count = read(filedes[0], buffer, sizeof(buffer) - 1);
    if (count == -1) {
      if (errno == EINTR) {
        continue;
      } else {
        perror("read");
        exit(1);
      }
    } else if (count == 0) {
      break;
    } else {
      buffer[count] = 0;
      ss << (char*)buffer;
    }
  }
  close(filedes[0]);

  int WaitStatus;
  auto WaitResult = waitpid(pid, &WaitStatus, 0);
  assert(WaitResult == pid && "Error on waitpid?");
  auto ExitCode = WEXITSTATUS(WaitStatus);
  return {Exe, ss.str(), ExitCode};
}

std::string Utils::buildShellCmd(const std::string &Exe,
                                 const std::vector<std::string> &Args) {

  std::stringstream ShellCmdStream;

  ShellCmdStream << "\"" << Exe << "\"";
  for (const auto &Arg : Args) {
    ShellCmdStream << " \"";
    for (char c : Arg) {
      if (c == '\"') {
        ShellCmdStream << "\\\"";
      } else {
        ShellCmdStream << c;
      }
    }
    ShellCmdStream << "\"";
  }

  return ShellCmdStream.str();
}

CmdResult Utils::runRawCmd(const std::string &ShellCmd) {
  assert(false);
}

std::vector<std::string> Utils::listFiles(const std::string &Dir,
                                          bool Recursive) {
  std::vector<std::string> Result;

  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir(Dir.c_str())) != nullptr) {
    while ((ent = readdir(dir)) != nullptr) {
      std::string Path(ent->d_name);
      std::string FullPath = Dir + "/" + Path;
      if (ent->d_type == DT_REG)
        Result.push_back(FullPath);
      if (ent->d_type == DT_DIR && Recursive && Path != "." && Path != "..") {
        auto Children = listFiles(FullPath);
        Result.insert(Result.end(), Children.begin(), Children.end());
      }
    }
    closedir(dir);
  } else {
    assert(false);
  }

  return Result;
}


std::string Utils::readFile(const std::string &Path) {
  std::ifstream In(Path);
  std::string Result;

  In.seekg(0, std::ios::end);
  Result.reserve(static_cast<unsigned long>(In.tellg()));
  In.seekg(0, std::ios::beg);

  Result.assign((std::istreambuf_iterator<char>(In)),
                std::istreambuf_iterator<char>());
  return Result;
}

std::size_t Utils::sizeOfDir(const std::string &Path) {
  CmdResult DuResult = runCmd("du", {"-sb", Path});
  DuResult.assumeGood();
  std::istringstream ISS(DuResult.Stdout);
  std::size_t Result;
  ISS >> Result;
  return Result;
}

bool Utils::stringEndsWith(const std::string &Str, const std::string &Suffix) {
  if (Str.length() >= Suffix.length()) {
    return (0 == Str.compare(Str.length() - Suffix.length(), Suffix.length(),
                             Suffix));
  } else {
    return false;
  }
}

bool Utils::fileExists(const std::string &Path) {
  std::ifstream F(Path);
  return F.good();
}
