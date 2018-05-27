#include <iostream>
#include <cassert>
#include <unistd.h>
#include <NativePasses/DeleteLinePass.h>
#include "Nils.h"
#include "Utils.h"

namespace {
  struct SaveWorkingDir {
    char *Dir;

    explicit SaveWorkingDir(const char *Backup) {
      Dir = get_current_dir_name();
      chdir(Backup);
    }
    ~SaveWorkingDir() {
      chdir(Dir);
      free(Dir);
    }
  };
}

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
    SaveWorkingDir RAII("/");
    Utils::copyDir(TestDir, DirToReduce);
  }
}

std::string Nils::createTmpDir() {
  Utils::copyDir(DirToReduce, TmpDir);
  return TmpDir;
}

void Nils::runPassOnDir(const std::string &Dir) {
  DeleteLinePass P;
  static std::size_t Ran = 0;
  ++Ran;
  P.runOnDir({Ran, Dir});
}
