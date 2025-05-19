#include "copy_files.hpp"
#include "flags.hpp"

#include <atomic>
#include <filesystem>
#include <functional>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

namespace fs = std::filesystem;

std::atomic_ulong copy_count(0);

void copy_files(std::unordered_set<fs::path> &src_files,
                std::string &format_str, std::string &dest_dir,
                int num_threads) {
    std::vector<std::thread> threads(num_threads);
    std::mutex src_files_mutex;
    PathBuilder path_builder(format_str);

    for (int i = 0; i < num_threads; i++) {
        threads[i] = std::thread(copy_files_thread, std::ref(src_files),
                                 std::ref(src_files_mutex),
                                 std::ref(path_builder), std::ref(dest_dir));
    }

    for (int i = 0; i < num_threads; i++) {
        threads[i].join();
    }

    if (!preview_flag) {
        fprintf(stdout, "Copied %lu files to '%s'\n", copy_count.load(),
                dest_dir.c_str());
    } else {
        fprintf(stdout, "Preview: %lu files would have been copied to '%s'\n",
                copy_count.load(), dest_dir.c_str());
    }
}

void copy_files_thread(std::unordered_set<std::filesystem::path> &src_files,
                       std::mutex &src_files_mutex, PathBuilder &path_builder,
                       std::string &dest_dir) {
    fs::path src_file_path;
    std::string dest_file_path;
    std::string dest_file_dir;
    fs::copy_options copy_options = replace_flag
                                        ? fs::copy_options::overwrite_existing
                                        : fs::copy_options::skip_existing;
    while (1) {
        src_files_mutex.lock();
        if (!src_files.empty()) {
            src_file_path = *src_files.begin();
            src_files.erase(src_file_path);
            src_files_mutex.unlock();
            try {
                dest_file_path =
                    dest_dir + path_builder.build_path(src_file_path);
                dest_file_dir =
                    dest_file_path.substr(0, dest_file_path.rfind('/'));
                if (!preview_flag) {
                    fs::create_directories(dest_file_dir);
                    if (fs::copy_file(src_file_path, dest_file_path,
                                      copy_options)) {
                        copy_count++;
                        if (verbose_flag) {
                            fprintf(stdout, "%s\n", dest_file_path.c_str());
                        }
                    } else {
                        if (fs::exists(dest_file_path)) {
                            if (verbose_flag) {
                                fprintf(stdout,
                                        "'%s' already exists, skipped\n",
                                        dest_file_path.c_str());
                            }
                        } else {
                            fprintf(stderr,
                                    "Error: file '%s' could not be copied\n",
                                    src_file_path.c_str());
                        }
                    }
                } else {
                    if (!fs::exists(dest_file_path)) {
                        fprintf(stdout, "%s\n", dest_file_path.c_str());
                    }
                    copy_count++;
                }
            } catch (std::invalid_argument err) {
                // fprintf(stderr, "%s\n", err.what());
                continue;
            } catch (fs::filesystem_error err) {
                fprintf(stderr, "%s\n", err.what());
            }
        } else {
            src_files_mutex.unlock();
            break;
        }
    }

    return;
}
