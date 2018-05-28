#include <iostream>
#include <cassert>
#include <unistd.h>
#include <NativePasses/DeleteLinePass.h>
#include <NativePasses/DeleteCharRangePass.h>
#include <chrono>
#include <NativePasses/RemoveTokenPass.h>
#include <NativePasses/RenameTokenPass.h>
#include "Nils.h"
#include "Utils.h"
#include "PassResult.h"
#include "ExecutablePass.h"


Nils::Nils(const std::string &DirToReduce) : DirToReduce(DirToReduce) {
  PassMgr.addPass(new DeleteLinePass());
  PassMgr.addPass(new DeleteCharRangePass(1));
  PassMgr.addPass(new DeleteCharRangePass(5));
  PassMgr.addPass(new DeleteCharRangePass(20));
  PassMgr.addPass(new DeleteCharRangePass(50));
  PassMgr.addPass(new DeleteCharRangePass(1000));
  PassMgr.addPass(new RemoveTokenPass());
  PassMgr.addPass(new RenameTokenPass());

  TmpDir += std::to_string(getppid());

  std::string TestDir = createTmpDir();
  std::string TestCmd = TestDir + "/nils.sh";
  CmdResult TestResult = Utils::runCmd(TestCmd, {}, TestDir);
  if (TestResult.ExitCode != 0) {
    assert(false);
  }
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

  Utils::deleteFile(TestCmd);

  PassResult Result;
  const Pass *P;
  Result.DirSizeChange = static_cast<long long>(Utils::sizeOfDir(TestDir));
  {
    MeasureTime<std::chrono::nanoseconds> RAII(Result.PassTime);
    P = runPassOnDir(TestDir);
    Result.UsedPass = P;
  }

  Result.DirSizeChange -= static_cast<long long>(Utils::sizeOfDir(TestDir));
  Result.DirSizeChange *= -1;

  Utils::copyFile(DirToReduce + "/nils.sh", TestCmd);

  CmdResult TestResult = Utils::runCmd(TestCmd, {}, TestDir);
  Result.Success = TestResult.ExitCode == 0 && Result.DirSizeChange < 0;

  PassMgr.feedback(P, Result);

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
  unsigned MaxErrorSequence = 400;
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
      std::cerr << File << std::endl;
      auto *P = new ExecutablePass(File);
      PassMgr.addPass(P);
    }
  }
}

