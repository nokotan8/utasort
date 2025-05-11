#include <filesystem>
#include <string>
#include <unordered_set>
int get_files(std::filesystem::path source_dir,
              std::unordered_set<std::string> &src_files) {
    std::unordered_set<std::string> files;

    for (const auto &entry :
         std::filesystem::recursive_directory_iterator(source_dir)) {
        if (!std::filesystem::is_directory(entry)) {
            files.insert(entry.path());
        }
    }

    return 0;
}
