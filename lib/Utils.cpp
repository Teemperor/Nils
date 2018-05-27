#include "Utils.h"

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

#ifdef PRINT_DEBUG
    std::cout << "RUNNING:" << ShellCmd << std::endl;
    std::cout << " OUTPUT: " << Result.Stdout << std::endl;
    std::cout << " EXIT CODE: " << Result.ExitCode << std::endl;
#endif

  return Result;
}

