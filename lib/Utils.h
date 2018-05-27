#ifndef NILS_UTILS_H
#define NILS_UTILS_H

#include <string>
#include <vector>
#include <sstream>
#include <random>

struct CmdResult {
  std::string Stdout;
  int ExitCode;
};

class Utils {
public:

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

  static std::vector<std::string> listFiles(const std::string &Dir, bool Recursive = true);

  static std::string readFile(const std::string &Path);

  static std::size_t sizeOfDir(const std::string &Path);

  static bool stringEndsWith(const std::string &Str, const std::string &Suffix);
  static bool stringStartsWith(const std::string &Str, const std::string &Prefix) {
    return Str.find(Prefix) == 0;
  }

};

template<typename Out>
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

template<typename T>  std::size_t getRandomBelow(std::size_t Limit, T &Engine) {
  std::uniform_int_distribution<std::size_t> RandomIndex(0, Limit - 1);
  return RandomIndex(Engine);
}

template<typename T, typename Engine> typename T::value_type selectRandom(T Collection, Engine &E) {
  return Collection.at(getRandomBelow(Collection.size(), E));
}


#endif //NILS_UTILS_H
