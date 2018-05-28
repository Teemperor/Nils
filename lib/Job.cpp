#include "Job.h"
#include "Utils.h"
#include <chrono>

std::string Job::createTmpDir() {
  Utils::createDir(WorkingDir);
  Utils::copyDir(SourceDir, WorkingDir);
  return WorkingDir;
}

Job::Job(const std::string &SourceDir, const std::string &WorkingDir)
    : SourceDir(SourceDir), WorkingDir(WorkingDir) {}

PassResult Job::run(const Pass *P) {
  ++Seed;

  std::string TestDir = createTmpDir();
  std::string TestCmd = TestDir + "/nils.sh";

  Utils::deleteFile(TestCmd);

  PassResult Result;

  Result.DirSizeChange = static_cast<long long>(Utils::sizeOfDir(TestDir));
  if (P) {
    MeasureTime<std::chrono::nanoseconds> RAII(Result.PassTime);
    P->runOnDir({Seed, TestDir});
    Result.UsedPass = P;
  }

  Result.DirSizeChange -= static_cast<long long>(Utils::sizeOfDir(TestDir));
  Result.DirSizeChange *= -1;

  Utils::copyFile(SourceDir + "/nils.sh", TestCmd);

  CmdResult TestResult = Utils::runCmd(TestCmd, {}, TestDir);
  Result.Success = TestResult.ExitCode == 0;
  // If we have a pass, it's only an success if that pass reduces the size.
  if (P)
    Result.Success &= Result.DirSizeChange < 0;

  return Result;
}
