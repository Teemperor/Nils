#ifndef NILS_EXECUTABLEPASS_H
#define NILS_EXECUTABLEPASS_H

#include "Pass.h"

class ExecutablePass : public Pass {
  std::string ExePath;
  std::string Name;

public:
  explicit ExecutablePass(const std::string &ExePath);

  const char *getName() const override { return Name.c_str(); }

  void runOnDir(const PassRun &Run) const override;
};

#endif // NILS_EXECUTABLEPASS_H
