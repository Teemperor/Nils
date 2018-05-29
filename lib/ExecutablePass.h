#ifndef NILS_EXECUTABLEPASS_H
#define NILS_EXECUTABLEPASS_H

#include "Pass.h"

class RegisterException : public std::exception {
  std::string Msg;
public:
  explicit RegisterException(std::string PassName) {
    Msg = "Registering pass for executable " + PassName + " has failed.";
  }

  const char* what() const noexcept override {
    return Msg.c_str();
  }
};

class ExecutablePass : public Pass {
  std::string ExePath;
  std::string Name;

public:
  explicit ExecutablePass(const std::string &ExePath);

  const char *getName() const override { return Name.c_str(); }

  void runOnDir(const PassRun &Run) const override;
};

#endif // NILS_EXECUTABLEPASS_H
