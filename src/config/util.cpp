#include "config/util.h"

namespace kvscan {
  /**
   * ReadInt
   * readint from src with 4 bytes
   */
  int ReadInt(char *src) {
    int len = *reinterpret_cast<uint32_t *>(src);
    return len;
  }

  /**
   * ReadString
   * read string from src with n bytes
   */
  std::string ReadString(char *src, int len) {
    return std::string(src, src+len); 
  }
}
