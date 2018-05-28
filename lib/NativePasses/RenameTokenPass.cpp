#include "RenameTokenPass.h"
#include "PassUtils.h"
#include "RemoveTokenPass.h"
#include <Utils.h>
#include <fstream>

void RenameTokenPass::runOnDir(const PassRun &Run) const {
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
  Token SelectedToken = Tokens.at(Index);
  Token Replacement = SelectedToken;
  Replacement.Content.resize(1);

  for (Token &T : Tokens) {
    if (T == SelectedToken) {
      T = Replacement;
    }
  }

  std::ofstream Out(File);
  Out << PassUtils::tokenToStr(Tokens);
}
