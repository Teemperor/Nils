#ifndef NILS_NILS_H
#define NILS_NILS_H


#include <string>

class Nils {
  std::string DirToReduce;
  std::string TmpDir = "/tmp/nils";

  std::string createTmpDir();

  void runPassOnDir(const std::string &Dir);

public:
  explicit Nils(const std::string &DirToReduce) : DirToReduce(DirToReduce) {
  }

  void iter();
};


#endif //NILS_NILS_H
