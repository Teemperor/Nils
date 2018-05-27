#include "ExecutablePass.h"
#include "Utils.h"


ExecutablePass::ExecutablePass(const std::string &ExePath) : ExePath(ExePath) {
}

void ExecutablePass::runOnDir(const PassRun &Run) const {
  Utils::runCmd(ExePath, {}, Run.Dir);
}
