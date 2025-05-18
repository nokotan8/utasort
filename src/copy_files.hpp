#include "PathBuilder.hpp"
#include <filesystem>
#include <mutex>
#include <string>
#include <unordered_set>

void copy_files(std::unordered_set<std::filesystem::path> &src_files,
                std::string &format_str, std::string &dest_dir,
                int num_threads);

void copy_files_thread(std::unordered_set<std::filesystem::path> &src_files,
                       std::mutex &src_files_mutex, PathBuilder &path_builder,
                       std::string &dest_dir);
