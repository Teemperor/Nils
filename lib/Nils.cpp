#include <iostream>
#include "Nils.h"
#include "Utils.h"

void Nils::iter() {
  std::string TestCmd = DirToReduce + "/nils.sh";
  std::cout << "Running " << TestCmd << std::endl;
  CmdResult TestResult = Utils::runCmd(TestCmd, {}, DirToReduce);
  std::cout << TestResult.ExitCode << std::endl;
}
