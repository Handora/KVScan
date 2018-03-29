#include "config/util.h"

namespace kvscan {

  int ReadInt(char *src) {
    int len = *reinterpret_cast<uint32_t *>(src);
    return len;
  }

  std::string ReadString(char *src, int len) {
    return std::string(src, src+len); 
  }
}
