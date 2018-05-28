#ifndef NILS_PASSMANAGER_H
#define NILS_PASSMANAGER_H

#include <vector>
#include <Pass.h>
#include <Utils.h>
#include "PassResult.h"

class PassManager {
  std::vector<Pass *> Passes;

  class PassRecord {
    std::size_t Points = 1;
    std::size_t FailedLastNTimes = 0;
    const Pass *P;
  public:
    explicit PassRecord(const Pass *P) : P(P) {
    }
    const Pass *getPass() const {
      return P;
    }
    std::size_t getPoints() const {
      return Points;
    }
    void feedback(const PassResult &Result) {
      if (Result.Success)
        FailedLastNTimes = 0;
      else
        FailedLastNTimes++;
    }
    std::size_t getFailedLastNTimes() const {
      return FailedLastNTimes;
    }
    void setPoints(std::size_t V) {
      Points = V;
      if (Points == 0)
        Points = 1;
    }
  };

  PassRecord &getRecordForPass(const Pass *P);

  std::vector<PassRecord> PassRecords;

  std::mt19937 Eng;
public:
  PassManager() : Eng(0) {
  }

  void dumpStats();

  void addPass(Pass *P) {
    Passes.push_back(P);
    PassRecords.emplace_back(P);
  }

  void feedback(const Pass *P, const PassResult &Result);

  const Pass *getNextPass();
};


#endif //NILS_PASSMANAGER_H
