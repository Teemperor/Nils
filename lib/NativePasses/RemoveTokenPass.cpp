#include <Utils.h>
#include <fstream>
#include "RemoveTokenPass.h"
#include "PassUtils.h"

void RemoveTokenPass::runOnDir(const PassRun &Run) const {
  auto Files = Utils::listFiles(Run.Dir);

  if (Files.empty())
    return;

  std::mt19937 Eng(Run.Rng);

  auto File = selectRandom(Files, Eng);

  std::string Content = Utils::readFile(File);
  if (Content.empty())
    return;

  auto Tokens = PassUtils::tokenize(Content);
  if (Tokens.empty())
    return;

  auto Index = getRandomBelow(Tokens.size(), Eng);
  Tokens.erase(Tokens.begin() + Index);

  std::ofstream Out(File);
  Out << PassUtils::tokenToStr(Tokens);
}
