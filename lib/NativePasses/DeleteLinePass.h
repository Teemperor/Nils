#ifndef NILS_DELETELINEPASS_H
#define NILS_DELETELINEPASS_H


#include <Pass.h>

class DeleteLinePass : public Pass {
public:
  const char *getName() const override {
    return "DeleteLine";
  }
  void runOnDir(const PassRun &Run) const override;
};


#endif //NILS_DELETELINEPASS_H
