#ifndef NILS_NILS_H
#define NILS_NILS_H

#include <string>
#include <functional>
#include "PassManager.h"
#include "PassResult.h"

class Nils {
  std::string DirToReduce;
  std::string TmpDir;

  PassManager PassMgr;

  std::function<void(const PassResult &)> Callback;

public:
  explicit Nils(const std::string &DirToReduce);

  void loadPassesFromDir(const std::string &Path);

  void setCallback(const std::function<void(const PassResult&)> &C) {
    Callback = C;
  }

  void run();
  PassResult iter();
};


#endif //NILS_NILS_H
