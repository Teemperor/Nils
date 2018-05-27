#ifndef NILS_PASSMANAGER_H
#define NILS_PASSMANAGER_H

#include <vector>
#include <Pass.h>
#include <Utils.h>

class PassManager {
  std::vector<Pass *> Passes;

  class PassRecord {
    std::size_t Weight = 1;
    const Pass *P;
  public:
    explicit PassRecord(const Pass *P) : P(P) {
    }
    const Pass *getPass() {
      return P;
    }
  };

  std::vector<PassRecord> PassRecords;

  std::mt19937 Eng;
public:
  PassManager() : Eng(0) {
  }

  void addPass(Pass *P) {
    Passes.push_back(P);
    PassRecords.emplace_back(P);
  }

  const Pass *getNextPass() {
    if (Passes.empty())
      return nullptr;
    return selectRandom(PassRecords, Eng).getPass();
  }
};


#endif //NILS_PASSMANAGER_H
