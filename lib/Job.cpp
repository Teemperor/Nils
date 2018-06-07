#include "Job.h"
#include "Utils.h"
#include <chrono>
#include <cassert>

std::string Job::createTmpDir() {
  Utils::createDir(WorkingDir);
  Utils::copyDir(SourceDir, WorkingDir);
  return WorkingDir;
}

Job::Job(const std::string &SourceDir, const std::string &WorkingDir,
         std::size_t Seed)
    : SourceDir(SourceDir), WorkingDir(WorkingDir), Seed(Seed) {
  assert(WorkingDir.back() != '/' && "Path shouldn't end with /");
  BackupDir = WorkingDir + "-backup";
  Utils::createDir(BackupDir);
}

PassResult Job::run(const Pass *PassToRun) {
  start(PassToRun);
  return join();
}

struct SaveGitRepo {
  bool Valid = false;
  std::string Dir, BackupDir;
  SaveGitRepo(const std::string &Dir, const std::string &BackupDir)
      : Dir(Dir), BackupDir(BackupDir) {
    Valid = Utils::fileExists(Dir + "/.git/HEAD");
    if (Valid)
      Utils::moveDir(Dir + "/.git", BackupDir + "/.git");
  }
  ~SaveGitRepo() {
    if (Valid)
      Utils::moveDir(BackupDir + "/.git", Dir + "/.git");
  }
};

void Job::start(const Pass *PassToRun) {
  LastPass = PassToRun;
  ++Seed;

  TestDir = createTmpDir();
  TestCmd = TestDir + "/nils.sh";

//#ifndef NDEBUG
//  Utils::runCmd(TestCmd, {}, TestDir).assumeGood();
//#endif

  Utils::deleteFile(TestCmd);

  Result.UsedPass = PassToRun;
  Result.WorkingDir = WorkingDir;

  {
    SaveGitRepo Guard(TestDir, BackupDir);

    Result.DirSizeChange = static_cast<long long>(Utils::sizeOfDir(TestDir));
    if (PassToRun) {
      MeasureTime<std::chrono::nanoseconds> RAII(Result.PassTime);
      PassToRun->runOnDir({Seed, TestDir});
    }
    Result.DirSizeChange -= static_cast<long long>(Utils::sizeOfDir(TestDir));
    Result.DirSizeChange *= -1;
  }

  Utils::copyFile(SourceDir + "/nils.sh", TestCmd);

  if (LastPass) {
    Utils::runCmd("git", {"add", "."}, TestDir).assumeGood();
    Utils::runCmd("git", {"commit", "--allow-empty", "-am", LastPass->getName()}, TestDir).assumeGood();
    Utils::moveDir(TestDir + "/.git", BackupDir + "/.git");
  }

  Proc = Process(TestCmd, {}, TestDir);
}

PassResult Job::join() {
  Proc.wait();
  Result.Success = (Proc.getExitCode() == 0);
  if (Result.Success && LastPass) {
    Utils::moveDir(BackupDir + "/.git", TestDir + "/.git");
    Utils::runCmd("git", {"clean", "-fdx"}, TestDir).assumeGood();
  }
  // If we have a pass, it's only an success if that pass reduces the size.
  if (LastPass)
    Result.Success &= Result.DirSizeChange < 0;

  return Result;
}
