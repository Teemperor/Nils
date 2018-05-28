#include <cassert>
#include "PassManager.h"

void PassManager::feedback(const Pass *P, const PassResult &Result) {
  auto &Record = getRecordForPass(P);
  if (Result.Success && Result.DirSizeChange < 0) {
    std::size_t AdditionalPoints = -Result.DirSizeChange;

    AdditionalPoints *= 4;
    AdditionalPoints += 40;

    Record.setPoints(Record.getPoints() + Result.DirSizeChange);
  } else {
    Record.setPoints(Record.getPoints() / 2);
  }
}

const Pass *PassManager::getNextPass() {
  assert(!Passes.empty());
  if (Passes.empty())
    return nullptr;

  std::size_t PointsSum = 0;
  for (auto &E : PassRecords) {
    PointsSum += E.getPoints();
  }

  std::size_t SelectedPassPoints = getRandomBelow(PointsSum, Eng);

  for (auto &E : PassRecords) {
    if (E.getPoints() > SelectedPassPoints) {
      return E.getPass();
    } else {
      SelectedPassPoints -= E.getPoints();
    }
    PointsSum += E.getPoints();
  }
  assert(false && "Run out of points for passes?");
}

PassManager::PassRecord &PassManager::getRecordForPass(const Pass *P) {
  for (auto &E : PassRecords) {
    if (E.getPass() == P)
      return E;
  }
  assert(false);
}
