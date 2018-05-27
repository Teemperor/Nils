#ifndef NILS_NILS_H
#define NILS_NILS_H


#include <string>

class Nils {
  std::string DirToReduce;
  std::string TmpDir = "/tmp/nils";

public:
  explicit Nils(const std::string &DirToReduce) : DirToReduce(DirToReduce) {
  }

  void iter();
};


#endif //NILS_NILS_H
