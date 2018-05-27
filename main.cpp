#include <unistd.h>
#include <Nils.h>

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
  N.iter();
}