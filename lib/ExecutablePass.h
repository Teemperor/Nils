#ifndef NILS_EXECUTABLEPASS_H
#define NILS_EXECUTABLEPASS_H


#include "Pass.h"

class ExecutablePass  : public Pass {
  std::string ExePath;
public:
  explicit ExecutablePass(const std::string &ExePath);

  void runOnDir(const PassRun &Run) const override;
};


#endif //NILS_EXECUTABLEPASS_H
