#include "Nils.h"
#include "ExecutablePass.h"
#include "Job.h"
#include "PassResult.h"
#include "Utils.h"
#include <NativePasses/DeleteCharRangePass.h>
#include <NativePasses/DeleteLinePass.h>
#include <NativePasses/RemoveTokenPass.h>
#include <NativePasses/RenameTokenPass.h>
#include <cassert>
#include <chrono>
#include <iostream>
#include <unistd.h>

Nils::Nils(const std::string &DirToReduce) : DirToReduce(DirToReduce) {
  PassMgr.addPass(new DeleteLinePass());
  PassMgr.addPass(new DeleteCharRangePass(1));
  PassMgr.addPass(new DeleteCharRangePass(5));
  PassMgr.addPass(new DeleteCharRangePass(20));
  PassMgr.addPass(new DeleteCharRangePass(50));
  PassMgr.addPass(new DeleteCharRangePass(1000));
  PassMgr.addPass(new RemoveTokenPass());
  PassMgr.addPass(new RenameTokenPass());

  TmpDir = "/tmp/nils" + std::to_string(getppid());

  auto InitTestDir = TmpDir + "-inittest";
  Job TestJob(DirToReduce, InitTestDir);
  auto TestResult = TestJob.run(nullptr);
  if (!TestResult.Success) {
    std::cerr << "When running the 'nils.sh' test file in the directory "
              << InitTestDir
              << " for testing reasons, it did not return 0. "
                 "Please ensure your nils.sh file actually returns 0 in its "
                 "current state";
    abort();
  }
}

PassResult Nils::iter() {
  const Pass *P = PassMgr.getNextPass();

  Job J(DirToReduce, TmpDir);
  auto Result = J.run(P);
  PassMgr.feedback(P, Result);

  if (Result.Success) {
    SaveWorkingDir RAII("/");
    Utils::copyDir(TmpDir, DirToReduce);
  }

  return Result;
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
