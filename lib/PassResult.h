#ifndef NILS_PASSRESULT_H
#define NILS_PASSRESULT_H

#include <string>
#include <cstddef>

struct PassResult {
  long long DirSizeChange = 0;
  std::size_t PassTime = 0;
  std::string PassName;
  bool Success = false;
};


#endif //NILS_PASSRESULT_H
