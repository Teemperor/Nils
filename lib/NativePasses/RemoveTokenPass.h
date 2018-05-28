#ifndef NILS_REMOVETOKENPASS_H
#define NILS_REMOVETOKENPASS_H

#include <Pass.h>

class RemoveTokenPass : public Pass {
public:
  const char *getName() const override { return "RemoveToken"; }
  void runOnDir(const PassRun &Run) const override;
};

#endif // NILS_REMOVETOKENPASS_H
