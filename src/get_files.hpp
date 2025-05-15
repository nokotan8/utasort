#include <filesystem>
#include <unordered_set>
int get_files(std::filesystem::path source_dir,
              std::unordered_set<std::filesystem::path> &src_files);
