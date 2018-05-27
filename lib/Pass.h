#ifndef NILS_PASS_H
#define NILS_PASS_H

#include <string>

struct PassRun {
  std::size_t Rng;
  std::string Dir;
};

class Pass {
public:
  virtual void runOnDir(const PassRun &Run) = 0;
};

#endif //NILS_PASS_H
