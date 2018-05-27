#include <iostream>
#include <cassert>
#include <unistd.h>
#include <NativePasses/DeleteLinePass.h>
#include <NativePasses/DeleteCharRangePass.h>
#include <chrono>
#include "Nils.h"
#include "Utils.h"
#include "PassResult.h"
#include "ExecutablePass.h"


Nils::Nils(const std::string &DirToReduce) : DirToReduce(DirToReduce) {
  PassMgr.addPass(new DeleteLinePass());
  PassMgr.addPass(new DeleteCharRangePass());
}

namespace {
  template<typename Unit = std::chrono::microseconds>
  struct MeasureTime {
    std::size_t &Value;
    std::chrono::steady_clock::time_point Start;
    explicit MeasureTime(std::size_t &Value) : Value(Value) {
      Start = std::chrono::steady_clock::now();
    }
    ~MeasureTime() {
      auto Stop = std::chrono::steady_clock::now();
      Value = std::chrono::duration_cast<Unit>(Stop - Start).count();
    }
  };

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

PassResult Nils::iter() {
  std::string TestDir = createTmpDir();
  std::string TestCmd = TestDir + "/nils.sh";
  CmdResult TestResult = Utils::runCmd(TestCmd, {}, TestDir);
  if (TestResult.ExitCode != 0) {
    assert(false);
  }

  PassResult Result;
  Result.DirSizeChange = static_cast<long long>(Utils::sizeOfDir(TestDir));
  {
    MeasureTime<std::chrono::nanoseconds> RAII(Result.PassTime);
    const Pass *P = runPassOnDir(TestDir);
    Result.PassName = P->getName();
  }

  Utils::copyFile(DirToReduce + "/nils.sh", TestCmd);

  Result.DirSizeChange -= static_cast<long long>(Utils::sizeOfDir(TestDir));
  Result.DirSizeChange *= -1;

  TestResult = Utils::runCmd(TestCmd, {}, TestDir);
  Result.Success = TestResult.ExitCode == 0 && Result.DirSizeChange < 0;

  if (TestResult.ExitCode == 0) {
    SaveWorkingDir RAII("/");
    Utils::copyDir(TestDir, DirToReduce);
  }

  return Result;
}

std::string Nils::createTmpDir() {
  Utils::copyDir(DirToReduce, TmpDir);
  return TmpDir;
}

const Pass *Nils::runPassOnDir(const std::string &Dir) {
  static std::size_t Ran = 0;
  ++Ran;
  auto *P = PassMgr.getNextPass();
  P->runOnDir({Ran, Dir});
  return P;
}

void Nils::run() {
  unsigned MaxErrorSequence = 100;
  unsigned ErrorSequence = MaxErrorSequence;
  while (true) {
    PassResult R = iter();
    if (Callback) {
      Callback(R);
    }

    if (!R.Success) {
      ErrorSequence--;
      if (ErrorSequence == 0)
        break;
    } else {
      ErrorSequence = MaxErrorSequence;
    }
  }
}

void Nils::loadPassesFromDir(const std::string &Path) {
  auto Files = Utils::listFiles(Path, false);
  for (auto &File : Files) {
    if (Utils::stringEndsWith(File, ".NilsPass")) {
      auto *P = new ExecutablePass(File);
      PassMgr.addPass(P);
    }
  }
}

