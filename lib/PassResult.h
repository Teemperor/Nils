#ifndef NILS_PASSRESULT_H
#define NILS_PASSRESULT_H

#include "Pass.h"
#include <cstddef>
#include <string>

struct PassResult {
  long long DirSizeChange = 0;
  std::size_t PassTime = 0;
  const Pass *UsedPass = nullptr;
  bool Success = false;
};

#endif // NILS_PASSRESULT_H
