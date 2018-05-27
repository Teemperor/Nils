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

static std::vector<std::string> listFiles(const std::string &Dir) {
  std::vector<std::string> Result;

  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir(Dir.c_str())) != nullptr) {
    while ((ent = readdir (dir)) != nullptr) {
      if (ent->d_type == DT_REG)
        Result.push_back(Dir + "/" + ent->d_name);
    }
    closedir (dir);
  } else {
    assert(false);
  }

  return Result;
}

void DeleteLinePass::runOnDir(const PassRun &Run) {
  auto Files = listFiles(Run.Dir);

  if (Files.empty())
    return;

  std::mt19937 Eng(Run.Rng);
  std::uniform_int_distribution<std::size_t> RandomFile(0, Files.size() - 1);

  auto File = Files.at(RandomFile(Eng));

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
  std::uniform_int_distribution<std::size_t> RandomLineDist(0, Lines.size() - 1);
  std::size_t RemovedLine = RandomLineDist(Eng);

  std::ofstream Out(File);
  std::size_t Index = 0;
  for (auto &Line : Lines) {
    if (Index != RemovedLine)
      Out << Line;
    ++Index;
  }
}
