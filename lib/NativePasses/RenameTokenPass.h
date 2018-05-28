#ifndef NILS_RENAMETOKENPASS_H
#define NILS_RENAMETOKENPASS_H

#include <Pass.h>

class RenameTokenPass : public Pass {
public:
  const char *getName() const override { return "RenameToken"; }
  void runOnDir(const PassRun &Run) const override;
};

#endif // NILS_RENAMETOKENPASS_H
