#include <Nils.h>
#include <iostream>
#include <unistd.h>

static const char *Red = "\x1b[31m";
static const char *Green = "\x1b[32m";
static const char *Yellow = "\x1b[33m";
static const char *Blue = "\x1b[34m";
static const char *Magenta = "\x1b[35m";
static const char *Cyan = "\x1b[36m";

static std::string color(const char *Color, const std::string &Token) {
  static const char *Reset = "\x1b[0m";
  return Color + Token + Reset;
}

int main(int argv, char **argc) {

  std::vector<std::string> Args;
  for (int A = 1; A < argv; ++A) {
    Args.emplace_back(argc[A]);
  }

  std::string Dir;
  if (Args.front() != "--batch") {
    Dir = Args.front();
  } else {
    char *CurrentDir = get_current_dir_name();
    Dir = CurrentDir;
    free(CurrentDir);
  }

  if (!Utils::fileExists("nils.sh")) {
    std::cerr << "Could not find the test file 'nils.sh' in the directory '"
              << Dir << "'. Without a test file nils can't minimize.\n";
    return 1;
  }

  bool MinusBatchSpecified = false;
  for (const auto &Arg : Args) {
    if (Arg == "--batch")
      MinusBatchSpecified = true;
  }

  if (!MinusBatchSpecified) {
    std::cout << color(Red,
                       "<[WARNING]> <[WARNING]> <[WARNING]> <[WARNING]>\n");
    std::cout << "nils will reduce the directory '" << Dir << "' now, which can"
              << " cause that all \nfiles in it are irreversible deleted. Are"
              << " you sure you want to continue?\n";
    std::cout << "Note: You can skip this warning with the --batch flag.\n";
    std::cout << color(Red,
                       "<[WARNING]> <[WARNING]> <[WARNING]> <[WARNING]>\n");
    std::cout << "To continue, please enter the directory name:\n";
    std::cout.flush();
    while (true) {
      std::string Line;
      const auto &GotInput = std::getline(std::cin, Line);
      if (!GotInput)
        break;
      if (Line == Dir) {
        break;
      } else {
        std::cout << "Enter the directory to reduce ('" << Dir << "')\n";
        std::cout << "> ";
        std::cout.flush();
      }
    }
  }

  Nils N(Dir);

  // Load any passes from specified directories.
  if (getenv("NILS_PASS_DIRS")) {
    std::string PassDirStr(getenv("NILS_PASS_DIRS"));
    std::vector<std::string> PassDirs;
    splitStr(PassDirStr, ':', PassDirs);

    for (auto &PassDir : PassDirs) {
      N.loadPassesFromDir(PassDir);
    }
  }

  N.setCallback([](const PassResult &Result) {
    std::string Status =
        Result.Success ? color(Green, "[GOOD]") : color(Red, "[FAIL]");
    std::cout << Status << " " << Result.UsedPass->getName()
              << " Pass took: " << Result.PassTime << " and changed size by "
              << Result.DirSizeChange << std::endl;
    return;
  });

  N.run();
}