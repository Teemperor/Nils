#include "Nils.h"
#include "Utils.h"

void Nils::iter() {
  std::string TestCmd = DirToReduce + "/test.sh";
  Utils::runCmd(TestCmd, {}, DirToReduce);
}
