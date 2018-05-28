#ifndef NILS_JOB_H
#define NILS_JOB_H

#include "PassResult.h"
#include <string>

class Job {

  std::string SourceDir;
  std::string WorkingDir;
  std::string createTmpDir();
  std::size_t Seed = 0;

public:
  explicit Job(const std::string &SourceDir, const std::string &WorkingDir,
               std::size_t Seed);

  PassResult run(const Pass *PassToRun);
};

#endif // NILS_JOB_H
