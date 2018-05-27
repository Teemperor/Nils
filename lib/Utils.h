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
  static void deleteDir(const std::string &Path);
  static void copyDir(const std::string &Source, const std::string &Target);
  static void copyFile(const std::string &Source, const std::string &Target);

  static CmdResult runRawCmd(const std::string &Cmd);
  static CmdResult runCmd(const std::string &Exe,
                     const std::vector<std::string> &Args = {},
                     const std::string &WorkingDir = "");
  static std::string buildShellCmd(const std::string &Exe,
                                   const std::vector<std::string> &Args = {});
};


#endif //NILS_UTILS_H
