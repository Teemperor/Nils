#ifndef NILS_DELETELINEPASS_H
#define NILS_DELETELINEPASS_H


#include <Pass.h>

class DeleteLinePass : public Pass {
public:
  void runOnDir(const PassRun &Run) override;
};


#endif //NILS_DELETELINEPASS_H