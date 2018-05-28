#include "Job.h"
#include "Utils.h"
#include <chrono>

std::string Job::createTmpDir() {
  Utils::createDir(WorkingDir);
  Utils::copyDir(SourceDir, WorkingDir);
  return WorkingDir;
}

Job::Job(const std::string &SourceDir, const std::string &WorkingDir,
         std::size_t Seed)
    : SourceDir(SourceDir), WorkingDir(WorkingDir), Seed(Seed) {}

PassResult Job::run(const Pass *PassToRun) {
  start(PassToRun);
  return join();
}

void Job::start(const Pass *PassToRun) {
  LastPass = PassToRun;
  ++Seed;

  TestDir = createTmpDir();
  TestCmd = TestDir + "/nils.sh";

#ifndef NDEBUG
  Utils::runCmd(TestCmd, {}, TestDir).assumeGood();
#endif

  Utils::deleteFile(TestCmd);

  Result.UsedPass = PassToRun;
  Result.WorkingDir = WorkingDir;

  Result.DirSizeChange = static_cast<long long>(Utils::sizeOfDir(TestDir));
  if (PassToRun) {
    MeasureTime<std::chrono::nanoseconds> RAII(Result.PassTime);
    PassToRun->runOnDir({Seed, TestDir});
  }

  Result.DirSizeChange -= static_cast<long long>(Utils::sizeOfDir(TestDir));
  Result.DirSizeChange *= -1;

  Utils::copyFile(SourceDir + "/nils.sh", TestCmd);

  Proc = Process(TestCmd, {}, TestDir);
}

PassResult Job::join() {
  Proc.wait();
  Result.Success = (Proc.getExitCode() == 0);
  // If we have a pass, it's only an success if that pass reduces the size.
  if (LastPass)
    Result.Success &= Result.DirSizeChange < 0;

  return Result;
}
