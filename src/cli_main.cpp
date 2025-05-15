#include <cstdio>
#include <filesystem>
#include <getopt.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_set>

#include "PathBuilder.hpp"
#include "get_files.hpp"

namespace fs = std::filesystem;
static int verbose_flag = 0;
static int replace_flag = 0;

int main(int argc, char *argv[]) {
    std::string src_dir;
    std::string dest_dir;
    std::string format_str;
    bool replace = false;

    int opt_char;
    while (1) {
        static struct option long_opts[] = {
            {"verbose", no_argument, &verbose_flag, 1},
            {"replace", no_argument, &replace_flag, 1},

            {"source-dir", required_argument, 0, 's'},
            {"destination-dir", required_argument, 0, 'd'},
            {"format", required_argument, 0, 'f'},
            {0, 0, 0, 0}

        };

        int opt_index = 0;
        opt_char = getopt_long(argc, argv, "s:d:f:r", long_opts, &opt_index);

        if (opt_char == -1)
            break;

        switch (opt_char) {
        case 0:
            // if (long_opts[opt_index].flag != 0)
            //     break;
            break;
        case 's': {
            src_dir = optarg;
            while (src_dir.back() == '/') {
                src_dir.pop_back();
            }
            src_dir = src_dir + '/';

            if (!fs::exists(src_dir) || !fs::is_directory(src_dir)) {
                fprintf(stderr,
                        "Source directory '%s' does not exist or is not a "
                        "directory\n",
                        optarg);
                return 1;
            }
            break;
        }
        case 'd': {
            dest_dir = optarg;
            while (dest_dir.back() == '/') {
                dest_dir.pop_back();
            }
            dest_dir = dest_dir + '/';

            if (fs::exists(dest_dir)) {
                if (!fs::is_directory(dest_dir)) {
                    fprintf(stderr,
                            "Destination directory '%s' is not a directory\n",
                            optarg);
                    return 1;
                }
            } else {
                if (fs::create_directories(dest_dir)) {
                    printf("Destination directory '%s' created\n", optarg);
                } else {
                    fprintf(stderr,
                            "Could not create destination directory '%s'",
                            optarg);
                    return 1;
                }
            }
            break;
        }
        case 'f':
            format_str = optarg;
            break;
        case '?':
            break;
        default:
            abort();
        }
    }

    if (!src_dir.length()) {
        fprintf(stderr, "Source directory not specified\n");
        return 1;
    } else if (!format_str.length()) {
        fprintf(stderr, "Format not specified\n");
        return 1;
    }

    if (!dest_dir.length()) {
        dest_dir = src_dir;
    }

    // Add all files paths to a set
    std::unordered_set<std::filesystem::path> src_files;
    get_files(src_dir, src_files);

    while (format_str[0] == '/') {
        format_str = format_str.substr(1);
    }
    while (format_str.back() == '/') {
        format_str.pop_back();
    }

    try {
        PathBuilder path_builder(format_str);
        const std::string song_path =
            dest_dir + path_builder.build_path(*src_files.begin());

        std::cout << song_path << '\n';
        // std::filesystem::create_directories(song_path.substr(0, song_path.rfind('/')));

    } catch (std::invalid_argument err) {
        fprintf(stderr, "%s\n", err.what());
        return 1;
    } catch (std::out_of_range err) {
        // Since the above code should pop all /'s from the array, this
        // shouldn't happen
        fprintf(stderr, "Damn you broke something\n");
        return 1;
    }

    return 0;
}
