#include <Utils.h>
#include <fstream>
#include "DeleteCharRangePass.h"

void DeleteCharRangePass::runOnDir(const PassRun &Run) const {
  auto Files = Utils::listFiles(Run.Dir);

  if (Files.empty())
    return;

  std::mt19937 Eng(Run.Rng);

  auto File = selectRandom(Files, Eng);

  std::string Content = Utils::readFile(File);

  auto StartIndex = getRandomBelow(Content.size() - 1, Eng);
  auto EndIndex = getRandomBelow(Content.size(), Eng);
  if (StartIndex >= EndIndex) {
    EndIndex = StartIndex + 1;
  }

  Content.erase(StartIndex, EndIndex - StartIndex);

  std::ofstream Out(File);
  Out << Content;
}
