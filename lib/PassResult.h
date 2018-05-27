#ifndef NILS_PASSRESULT_H
#define NILS_PASSRESULT_H


#include <cstddef>

struct PassResult {
  long long DirSizeChange = 0;
  std::size_t PassTime = 0;
  bool Success = false;
};


#endif //NILS_PASSRESULT_H
