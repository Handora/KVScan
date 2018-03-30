#pragma once

#include <cstdint>
#include <string>

namespace kvscan {
#define INVALID_PAGE_ID -1
#define NOFOUND_PAGE_ID -2
#define INITIAL_PAGE_ID -3
#define PAGE_SIZE 512

  typedef int page_id_t;
  int ReadInt(char *src);
  std::string ReadString(char *src, int len);

} // namespace kvscan
