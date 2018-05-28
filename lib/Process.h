#ifndef NILS_PROCESS_H
#define NILS_PROCESS_H

#include <string>
#include <vector>

class Process {
  std::string StdOut;
  int ExitCode = 0;
  std::string Exe;
  int filedes[2];
  pid_t pid;
public:
  Process() = default;
  Process(const std::string &Exe,
          const std::vector<std::string> &Args,
          const std::string &WorkingDir);

  void wait();

  const std::string &getStdOut() const {
    return StdOut;
  }

  int getExitCode() const {
    return ExitCode;
  }

  const std::string &getExe() const {
    return Exe;
  }
};


#endif //NILS_PROCESS_H
