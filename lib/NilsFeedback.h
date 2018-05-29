#ifndef NILS_NILSFEEDBACK_H
#define NILS_NILSFEEDBACK_H


#include <functional>
#include "PassResult.h"

class NilsFeedback {
  std::function<void(const PassResult &)> AppliedPass;
  std::function<void(const Pass *P)> RegisterPass;
  std::function<void(const Pass *P)> FailedToRegisterPass;
public:
  const std::function<void(const PassResult &)> &getAppliedPass() const {
    return AppliedPass;
  }

  void
  setAppliedPass(const std::function<void(const PassResult &)> &AppliedPass) {
    NilsFeedback::AppliedPass = AppliedPass;
  }

  const std::function<void(const Pass *)> &getRegisterPass() const {
    return RegisterPass;
  }

  void setRegisterPass(const std::function<void(const Pass *)> &RegisterPass) {
    NilsFeedback::RegisterPass = RegisterPass;
  }

  const std::function<void(const Pass *)> &getFailedToRegisterPass() const {
    return FailedToRegisterPass;
  }

  void setFailedToRegisterPass(
      const std::function<void(const Pass *)> &FailedToRegisterPass) {
    NilsFeedback::FailedToRegisterPass = FailedToRegisterPass;
  }
};


#endif //NILS_NILSFEEDBACK_H
