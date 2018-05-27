#ifndef NILS_UTILS_H
#define NILS_UTILS_H

#include <string>
#include <vector>

struct CmdResult {
  std::string Stdout;
  int ExitCode;
};

class Utils {
public:
  static void copyDir(const std::string &Source, const std::string &Target);
  static CmdResult runCmd(const std::string &Exe,
                     const std::vector<std::string> &Args = {},
                     const std::string &WorkingDir = "");
  static std::string buildShellCmd(const std::string &Exe,
                                   const std::vector<std::string> &Args = {});
};


#endif //NILS_UTILS_H
