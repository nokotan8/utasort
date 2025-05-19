#include "get_files.hpp"
#include <filesystem>
#include <unordered_set>

int get_files(std::filesystem::path source_dir,
              std::unordered_set<std::filesystem::path> &src_files) {

    for (const auto &entry :
         std::filesystem::recursive_directory_iterator(source_dir)) {
        if (std::filesystem::is_regular_file(entry)) {
            src_files.insert(entry.path());
        }
    }

    return 0;
}
