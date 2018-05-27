#ifndef NILS_NILS_H
#define NILS_NILS_H

#include <string>
#include <functional>
#include "PassManager.h"
#include "PassResult.h"

class Nils {
  std::string DirToReduce;
  std::string TmpDir = "/tmp/nils";

  PassManager PassMgr;

  std::string createTmpDir();

  void runPassOnDir(const std::string &Dir);

  std::function<void(const PassResult &)> Callback;

public:
  explicit Nils(const std::string &DirToReduce);

  void setCallback(const std::function<void(const PassResult&)> &C) {
    Callback = C;
  }

  void run();
  PassResult iter();
};


#endif //NILS_NILS_H
