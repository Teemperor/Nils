#include <cassert>
#include <iostream>
#include <iomanip>
#include "PassManager.h"

void PassManager::feedback(const Pass *P, const PassResult &Result) {
  auto &Record = getRecordForPass(P);
  Record.feedback(Result);

  if (Result.Success && Result.DirSizeChange < 0) {
    std::size_t AdditionalPoints = -Result.DirSizeChange;

    AdditionalPoints *= 4;
    AdditionalPoints += 40;

    Record.setPoints(Record.getPoints() + AdditionalPoints);
  } else {
    std::size_t FailPunishment = (Record.getFailedLastNTimes() / 10 + 1);
    Record.setPoints(Record.getPoints() / FailPunishment);
  }
}

const Pass *PassManager::getNextPass() {
  assert(!Passes.empty());
  if (Passes.empty())
    return nullptr;

  std::size_t PointsSum = 0;
  for (auto &E : PassRecords) {
    PointsSum += E.getTotalPoints();
  }

  std::size_t SelectedPassPoints = getRandomBelow(PointsSum, Eng);

  dumpStats();

  for (auto &E : PassRecords) {
    if (E.getTotalPoints() > SelectedPassPoints) {
      return E.getPass();
    } else {
      SelectedPassPoints -= E.getTotalPoints();
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

void PassManager::dumpStats() {
  std::cout << "STATS:\n";
  for (auto &E : PassRecords) {
    std::cout << " PASS: ";
    std::cout << std::setw(25) << std::left;
    std::cout << E.getPass()->getName() << " : ";
    std::cout << std::setw(10) << std::left << E.getTotalPoints();
    std::cout  << "(FailedN: " << E.getFailedLastNTimes() << ")\n";
  }
}
