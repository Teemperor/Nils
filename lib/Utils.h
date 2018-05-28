#ifndef NILS_UTILS_H
#define NILS_UTILS_H

#include <chrono>
#include <random>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

struct CmdResult {
  std::string Command;
  std::string Stdout;
  int ExitCode;
  void assumeGood() {
    if (ExitCode != 0)
      throw std::logic_error("Command unexpectedly failed with exit code "
                             + std::to_string(ExitCode) + ": "+ Command
       + ":\n" + Stdout);
  }
};

class Utils {
public:
  static void createDir(const std::string &Path);
  static void deleteDir(const std::string &Path);
  static void deleteFile(const std::string &Path);
  static void copyDir(const std::string &Source, const std::string &Target);
  static void copyFile(const std::string &Source, const std::string &Target);

  static CmdResult runRawCmd(const std::string &Cmd);
  static CmdResult runCmd(const std::string &Exe,
                          const std::vector<std::string> &Args = {},
                          const std::string &WorkingDir = "");
  static std::string buildShellCmd(const std::string &Exe,
                                   const std::vector<std::string> &Args = {});

  static std::vector<std::string> listFiles(const std::string &Dir,
                                            bool Recursive = true);

  static std::string readFile(const std::string &Path);

  static std::size_t sizeOfDir(const std::string &Path);

  static bool stringEndsWith(const std::string &Str, const std::string &Suffix);
  static bool stringStartsWith(const std::string &Str,
                               const std::string &Prefix) {
    return Str.find(Prefix) == 0;
  }

  static bool fileExists(const std::string &Path);
};

template <typename Out>
void splitStr(const std::string &S, char Delimiter, Out &Result) {
  std::string DelimStr(1, Delimiter);
  if (S.find(DelimStr) == std::string::npos) {
    Result.push_back(S);
    return;
  }
  std::stringstream SS(S);
  std::string Item;
  while (std::getline(SS, Item, Delimiter)) {
    Result.push_back(Item);
  }
}

template <typename T> std::size_t getRandomBelow(std::size_t Limit, T &Engine) {
  std::uniform_int_distribution<std::size_t> RandomIndex(0, Limit - 1);
  return RandomIndex(Engine);
}

template <typename T, typename Engine>
typename T::value_type selectRandom(T Collection, Engine &E) {
  return Collection.at(getRandomBelow(Collection.size(), E));
}

template <typename Unit = std::chrono::microseconds> struct MeasureTime {
  std::size_t &Value;
  std::chrono::steady_clock::time_point Start;
  explicit MeasureTime(std::size_t &Value) : Value(Value) {
    Start = std::chrono::steady_clock::now();
  }
  ~MeasureTime() {
    auto Stop = std::chrono::steady_clock::now();
    Value = std::chrono::duration_cast<Unit>(Stop - Start).count();
  }
};

struct SaveWorkingDir {
  char *Dir;

  explicit SaveWorkingDir(const char *Backup) {
    Dir = get_current_dir_name();
    chdir(Backup);
  }
  ~SaveWorkingDir() {
    chdir(Dir);
    free(Dir);
  }
};

#endif // NILS_UTILS_H
