#include "DeleteLinePass.h"

#include <dirent.h>
#include <cassert>
#include <vector>
#include <random>
#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>
#include <algorithm>
#include <Utils.h>


void DeleteLinePass::runOnDir(const PassRun &Run) {
  auto Files = Utils::listFiles(Run.Dir);

  if (Files.empty())
    return;

  std::mt19937 Eng(Run.Rng);

  auto File = selectRandom(Files, Eng);

  std::vector<std::string> Lines;

  {
    std::ifstream In(File);

    std::string Line;
    while (std::getline(In, Line))
    {
      Lines.push_back(Line);
    }
  }

  if (Lines.empty())
    return;

  std::size_t RemovedLine = getRandomBelow(Lines.size(), Eng);

  std::ofstream Out(File);
  std::size_t Index = 0;
  for (auto &Line : Lines) {
    if (Index != RemovedLine)
      Out << Line << "\n";
    ++Index;
  }
}
