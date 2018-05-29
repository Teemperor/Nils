#ifndef NILS_JOB_H
#define NILS_JOB_H

#include "PassResult.h"
#include "Process.h"
#include <string>

class Job {

  std::string SourceDir;
  std::string WorkingDir;
  std::string BackupDir;
  std::string createTmpDir();
  std::size_t Seed = 0;
  std::string TestDir;
  std::string TestCmd;
  PassResult Result;
  const Pass *LastPass = nullptr;

  Process Proc;

public:
  Job() = default;
  explicit Job(const std::string &SourceDir, const std::string &WorkingDir,
               std::size_t Seed);

  PassResult run(const Pass *PassToRun);
  void start(const Pass *PassToRun);
  PassResult join();
};

#endif // NILS_JOB_H
