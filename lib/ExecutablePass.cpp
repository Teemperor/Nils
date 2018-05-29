#include "ExecutablePass.h"
#include "Utils.h"

#include "json.hpp"

ExecutablePass::ExecutablePass(const std::string &ExePath) : ExePath(ExePath) {
  std::vector<std::string> Parts;
  splitStr(ExePath, '/', Parts);
  Name = Parts.back();

  std::string Suffix = ".NilsPass";
  if (Utils::stringEndsWith(Name, Suffix)) {
    Name.resize(Name.size() - Suffix.size());
  }

  CmdResult RegisterResult = Utils::runCmd(ExePath, {"--register"}, "/tmp");
  if (RegisterResult.ExitCode != 0) {
    throw RegisterException(ExePath);
  }

  auto RegInfo = nlohmann::json::parse(RegisterResult.Stdout);
}

void ExecutablePass::runOnDir(const PassRun &Run) const {
  auto Files = Utils::listFiles(Run.Dir);

  // TODO: Maybe we should still run the pass if we have no files?
  if (Files.empty())
    return;

  std::mt19937 Eng(Run.Rng);

  auto File = selectRandom(Files, Eng);

  Utils::runCmd(ExePath, {File}, Run.Dir);
}
