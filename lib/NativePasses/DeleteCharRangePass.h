#ifndef NILS_DELETECHARRANGEPASS_H
#define NILS_DELETECHARRANGEPASS_H


#include <Pass.h>

class DeleteCharRangePass : public Pass {
public:
  const char *getName() const override {
    return "DeleteCharRangePass";
  }
  void runOnDir(const PassRun &Run) const override;
};



#endif //NILS_DELETECHARRANGEPASS_H
