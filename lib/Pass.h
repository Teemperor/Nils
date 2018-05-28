#ifndef NILS_PASS_H
#define NILS_PASS_H

#include <string>

struct PassRun {
  std::size_t Rng;
  std::string Dir;
};

class Pass {
public:
  virtual const char *getName() const = 0;
  virtual void runOnDir(const PassRun &Run) const = 0;
};

#endif // NILS_PASS_H
