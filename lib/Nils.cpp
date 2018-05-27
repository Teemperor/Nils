#include <iostream>
#include <cassert>
#include <unistd.h>
#include "Nils.h"
#include "Utils.h"

void Nils::iter() {
  std::string TestDir = createTmpDir();
  std::string TestCmd = TestDir + "/nils.sh";
  CmdResult TestResult = Utils::runCmd(TestCmd, {}, TestDir);
  if (TestResult.ExitCode != 0) {
    assert(false);
  }
  runPassOnDir(TestDir);

  Utils::copyFile(DirToReduce + "/nils.sh", TestCmd);

  TestResult = Utils::runCmd(TestCmd, {}, TestDir);
  if (TestResult.ExitCode == 0) {
    char *Pwd = get_current_dir_name();
    chdir("/");
    Utils::copyDir(TestDir, DirToReduce);
    chdir(Pwd);
    free(Pwd);
  }
}

std::string Nils::createTmpDir() {
  Utils::copyDir(DirToReduce, TmpDir);
  return TmpDir;
}

void Nils::runPassOnDir(const std::string &Dir) {
  Utils::runRawCmd("cd " + Dir + " ; echo -n 'weird' > data");
}
