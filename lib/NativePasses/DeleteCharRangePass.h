#ifndef NILS_DELETECHARRANGEPASS_H
#define NILS_DELETECHARRANGEPASS_H


#include <Pass.h>

class DeleteCharRangePass : public Pass {
public:
  void runOnDir(const PassRun &Run) override;
};



#endif //NILS_DELETECHARRANGEPASS_H
