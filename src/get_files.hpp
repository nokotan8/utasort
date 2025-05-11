#include <filesystem>
#include <string>
#include <unordered_set>
int get_files(std::filesystem::path source_dir,
              std::unordered_set<std::string> &src_files);
