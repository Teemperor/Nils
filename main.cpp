#include <unistd.h>
#include <Nils.h>
#include <iostream>

int main(int argv, char **argc) {
  std::string Dir;
  if (argv <= 1) {
    char *CurrentDir = get_current_dir_name();
    Dir = CurrentDir;
    free(CurrentDir);
  } else {
    Dir = argc[1];
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

  N.setCallback([](const PassResult &Result){
    std::string Status = Result.Success ? "[ GOOD ]" : "[ FAIL ]";
    std::cout << Status << " " << Result.UsedPass->getName()
              << " Pass took: " << Result.PassTime << " and changed size by "
              << Result.DirSizeChange << std::endl;
    return;
  });

  N.run();
}