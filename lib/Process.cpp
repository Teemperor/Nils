#include "Process.h"

#include <unistd.h>
#include <iostream>
#include <wait.h>
#include <sstream>
#include <cassert>

Process::Process(const std::string &Exe, const std::vector<std::string> &Args,
                 const std::string &WorkingDir) : Exe(Exe) {

  if (pipe(filedes) == -1) {
    perror("pipe");
    exit(1);
  }

  std::vector<char *> CArgs;
  CArgs.push_back(const_cast<char *>(Exe.c_str()));
  for (const std::string &Arg : Args) {
    CArgs.push_back(const_cast<char *>(Arg.c_str()));
  }
  CArgs.push_back(nullptr);

  pid = fork();
  if (pid == -1) {
    perror("fork");
    exit(1);
  } else if (pid == 0) {
    if (!WorkingDir.empty())
      chdir(WorkingDir.c_str());
    while ((dup2(filedes[1], STDOUT_FILENO) == -1) && (errno == EINTR)) {}
    close(filedes[1]);
    close(filedes[0]);

    execvp(Exe.c_str(), CArgs.data());
    perror("execvp");
    _exit(1);
  }
  close(filedes[1]);
}

void Process::wait() {
  std::stringstream ss;
  // buffer and 0 terminate it.
  const std::size_t BufSize = 4096;
  char buffer[BufSize];
  buffer[BufSize - 1] = 0;

  while (true) {
    ssize_t count = read(filedes[0], buffer, BufSize - 1);
    if (count == -1) {
      if (errno == EINTR) {
        continue;
      } else {
        perror("read");
        exit(1);
      }
    } else if (count == 0) {
      break;
    } else {
      buffer[count] = 0;
      ss << (char *) buffer;
    }
  }
  close(filedes[0]);

  int WaitStatus;
  auto WaitResult = waitpid(pid, &WaitStatus, 0);
  assert(WaitResult == pid && "Error on waitpid?");
  ExitCode = WEXITSTATUS(WaitStatus);
  StdOut = ss.str();
}
