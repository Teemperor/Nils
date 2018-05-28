#ifndef NILS_DELETECHARRANGEPASS_H
#define NILS_DELETECHARRANGEPASS_H


#include <Pass.h>
#include <limits>

class DeleteCharRangePass : public Pass {
  std::size_t MaxSize;
  std::string Name;
public:
  DeleteCharRangePass(std::size_t MaxSize = std::numeric_limits<std::size_t>::max())
   : MaxSize(MaxSize) {
    Name = "DeleteCharRange(Max:" + std::to_string(MaxSize) + ")";
  }
  const char *getName() const override {
    return Name.c_str();
  }
  void runOnDir(const PassRun &Run) const override;
};



#endif //NILS_DELETECHARRANGEPASS_H
