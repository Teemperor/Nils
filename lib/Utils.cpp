#include "Utils.h"

#include <sstream>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

void Utils::copyDir(const std::string &Source, const std::string &Target) {
  runCmd("cp", {"-r", Source, Target});
}

CmdResult Utils::runCmd(const std::string &Exe,
                          const std::vector<std::string> &Args,
                          const std::string &WorkingDir) {
  const unsigned BufferSize = 128;
  std::array<char, BufferSize> buffer{};


  std::stringstream Result;
  std::string ShellCmd = buildShellCmd(Exe, Args);
  if (!WorkingDir.empty()) {
    ShellCmd = buildShellCmd("cd", {WorkingDir}) + " ; " + ShellCmd;
  }
  ShellCmd += " 2>&1";

  FILE *Pipe = popen(ShellCmd.c_str(), "r");

  if (!Pipe)
    throw std::runtime_error("popen() failed!");

  while (!feof(Pipe)) {
    if (fgets(buffer.data(), BufferSize, Pipe) != nullptr)
      Result << buffer.data();
  }

  return {Result.str(), WEXITSTATUS(pclose(Pipe))};
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
