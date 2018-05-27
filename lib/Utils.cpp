#include "Utils.h"


#include <dirent.h>
#include <cassert>
#include <vector>
#include <random>
#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

void Utils::deleteDir(const std::string &Path) {
  runCmd("rm", {"-rf", Path});
}

void Utils::copyDir(const std::string &Source, const std::string &Target) {
  deleteDir(Target);
  runCmd("cp", {"-r", Source, Target});
}

void Utils::copyFile(const std::string &Source, const std::string &Target) {
  runCmd("cp", {Source, Target});
}

CmdResult Utils::runCmd(const std::string &Exe,
                          const std::vector<std::string> &Args,
                          const std::string &WorkingDir) {


  std::string ShellCmd = buildShellCmd(Exe, Args);
  if (!WorkingDir.empty()) {
    ShellCmd = buildShellCmd("cd", {WorkingDir}) + " ; " + ShellCmd;
  }
  ShellCmd += " 2>&1";

  return runRawCmd(ShellCmd);
}

std::string Utils::buildShellCmd(const std::string &Exe,
                                 const std::vector<std::string> &Args) {

  std::stringstream ShellCmdStream;

  ShellCmdStream << "\"" << Exe << "\"";
  for (const auto& Arg : Args) {
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
  const unsigned BufferSize = 128;
  std::array<char, BufferSize> buffer{};
  std::stringstream Output;

  FILE *Pipe = popen(ShellCmd.c_str(), "r");

  if (!Pipe)
    throw std::runtime_error("popen() failed!");

  while (!feof(Pipe)) {
    if (fgets(buffer.data(), BufferSize, Pipe) != nullptr)
      Output << buffer.data();
  }

  CmdResult Result = {Output.str(), WEXITSTATUS(pclose(Pipe))};

//#define PRINT_DEBUG
#ifdef PRINT_DEBUG
    std::cout << "RUNNING:" << ShellCmd << std::endl;
    std::cout << " OUTPUT: " << Result.Stdout << std::endl;
    std::cout << " EXIT CODE: " << Result.ExitCode << std::endl;
#endif

  return Result;
}

std::vector<std::string> Utils::listFiles(const std::string &Dir,
                                          bool Recursive) {
  std::vector<std::string> Result;

  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir(Dir.c_str())) != nullptr) {
    while ((ent = readdir (dir)) != nullptr) {
      std::string Path(ent->d_name);
      std::string FullPath = Dir + "/" + Path;
      if (ent->d_type == DT_REG)
        Result.push_back(FullPath);
      if (ent->d_type == DT_DIR && Recursive &&
          Path != "." && Path != "..") {
        auto Children = listFiles(FullPath);
        Result.insert(Result.end(), Children.begin(), Children.end());
      }
    }
    closedir (dir);
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
  std::istringstream ISS(DuResult.Stdout);
  std::size_t Result;
  ISS >> Result;
  return Result;
}

