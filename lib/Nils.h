#ifndef NILS_NILS_H
#define NILS_NILS_H

#include "PassManager.h"
#include "PassResult.h"
#include "NilsFeedback.h"
#include <functional>
#include <string>

class Nils {
  std::string DirToReduce;
  std::string TmpDir;

  PassManager PassMgr;

  NilsFeedback Feedback;

public:
  explicit Nils(const std::string &DirToReduce);

  void loadPassesFromDir(const std::string &Path);

  NilsFeedback &getFeedback() {
    return Feedback;
  }

  void run();
  PassResult iter();
};

#endif // NILS_NILS_H
