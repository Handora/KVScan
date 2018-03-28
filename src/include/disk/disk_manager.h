#include <atomic>
#include <fstream>
#include <future>
#include <string>

namespace kvscan {
  typedef int page_id_t;
  class DiskManager {
  public:
    DiskManager(const std::string &file);
    ~DiskManager();

    void WritePage(page_id_t page_id, const char *page_data);
    void ReadPage(page_id_t page_id, char *page_data);
    
    page_id_t AllocatePage();
    void DeallocatePage(page_id_t page_id);
    
  private:
    int GetFileSize(const std::string &name);
    // stream to write db file
    std::fstream db_io_;
    std::string file_name_;
    std::atomic<page_id_t> next_page_id_;
  };

} // namespace kvscan
