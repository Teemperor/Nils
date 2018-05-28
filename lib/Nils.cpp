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
#include <thread>

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
  Job TestJob(DirToReduce, InitTestDir, 0);
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
  PassMgr.dumpStats();

  static std::size_t Ran = 0;

  std::size_t JobNum = 7;

  std::vector<Job *> Jobs;

  std::vector<PassResult> Results;

  for (std::size_t Index = 0; Index < JobNum; Index ++) {
    ++Ran;
    const Pass *P = PassMgr.getNextPass();
    assert(P && "Couldn't decide on pass?");
    std::string JobDir = TmpDir + "-j" + std::to_string(Index);

    Jobs.push_back(new Job(DirToReduce, JobDir, Ran));
    Jobs.back()->start(P);
  }

  for (Job *J : Jobs) {
    Results.push_back(J->join());
  }

  for (Job *J : Jobs)
    delete J;

  PassResult FinalResult;
  for (auto &Result : Results) {
    FinalResult = Result;
    PassMgr.feedback(Result.UsedPass, Result);
    if (Result.Success) {
      assert(!Result.WorkingDir.empty() && "Used working dir empty?");
      SaveWorkingDir RAII("/");
      Utils::copyDir(Result.WorkingDir, DirToReduce);
      break;
    }
  }


  return FinalResult;
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
