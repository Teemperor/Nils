#include "DeleteLinePass.h"

#include <Utils.h>
#include <algorithm>
#include <cassert>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <random>
#include <vector>

void DeleteLinePass::runOnDir(const PassRun &Run) const {
  auto Files = Utils::listFiles(Run.Dir);

  if (Files.empty())
    return;

  std::mt19937 Eng(Run.Rng);

  auto File = selectRandom(Files, Eng);

  std::vector<std::string> Lines;

  {
    std::ifstream In(File);

    std::string Line;
    while (std::getline(In, Line)) {
      Lines.push_back(Line);
    }
  }

  if (Lines.empty())
    return;

  std::size_t Start = getRandomBelow(Lines.size(), Eng);
  std::size_t End = getRandomBelow(Lines.size(), Eng);
  if (Start > End) {
    End = Start + 1;
  }

  std::ofstream Out(File);
  std::size_t Index = 0;
  for (auto &Line : Lines) {
    if (Index < Start || Index > End)
      Out << Line << "\n";
    ++Index;
  }
}
