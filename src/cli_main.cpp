#include <cstdio>
#include <filesystem>
#include <getopt.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_set>

#include "PathBuilder.hpp"
#include "flags.hpp"
#include "get_files.hpp"

namespace fs = std::filesystem;

int verbose_flag = 0;
int replace_flag = 0;
int move_flag = 0;

int main(int argc, char *argv[]) {
    std::string src_dir;
    std::string dest_dir;
    std::string format_str;

    int opt_char;
    while (1) {
        static struct option long_opts[] = {
            {"verbose", no_argument, &verbose_flag, 1},

            {"help", no_argument, 0, 'h'},
            {"source", required_argument, 0, 's'},
            {"dest", required_argument, 0, 'd'},
            {"format", required_argument, 0, 'f'},
            {"replace", no_argument, 0, 'r'},
            {0, 0, 0, 0}

        };

        int opt_index = 0;
        opt_char = getopt_long(argc, argv, "hs:d:f:r", long_opts, &opt_index);

        if (opt_char == -1)
            break;

        switch (opt_char) {
        case 0:
            // if (long_opts[opt_index].flag != 0)
            //     break;
            break;
        case 'h': {
            std::cout << "Usage: utasort -f <format_str> -s <source_dir> "
                         "[options]...\n";
            std::cout << "Required:\n";
            std::cout << "  -s, --source path\tPath to the source directory\n";
            std::cout << "  -f, --format string\tFormat string for the "
                         "directory structure\n";
            std::cout << "Optional:\n";
            std::cout << "      --verbose\t\tPrint more information, including "
                         "the source and destination of every file\n";
            std::cout << "  -d, --dest path\tPath to the destination "
                         "directory;\n";
            std::cout << "\t\t\tIf not specified, the source directory will be "
                         "used\n";
            std::cout << "  -r, --replace\t\tReplace files in the destination "
                         "directory if there are collisions;\n";
            std::cout << "\t\t\tIf not specified, existing files will not be "
                         "overwritten\n";
            std::cout << "Help:\n";
            std::cout << "  -h, --help\t\tShow this help message and exit\n";
            std::cout << "  -H, --format-help\tShow help for defining the "
                         "format string and exit\n";

            exit(1);
        }
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
                fprintf(stderr, "Destination directory '%s' does not exist",
                        optarg);
                return 1;
            }
            break;
        }
        case 'f':
            format_str = optarg;
            break;
        case 'r':
            replace_flag = 1;
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
    std::unordered_set<fs::path> src_files;
    get_files(src_dir, src_files);

    while (format_str[0] == '/') {
        format_str = format_str.substr(1);
    }
    while (format_str.back() == '/') {
        format_str.pop_back();
    }

    try {
        PathBuilder path_builder(format_str);
        fs::path src_file_path;
        std::string dest_file_path;
        std::string dest_file_dir;
        fs::copy_options copy_options =
            replace_flag ? fs::copy_options::overwrite_existing
                         : fs::copy_options::skip_existing;
        while (!src_files.empty()) {
            src_file_path = *src_files.begin();
            dest_file_path = dest_dir + path_builder.build_path(src_file_path);
            dest_file_dir = dest_file_path.substr(0, dest_file_path.rfind('/'));
            fs::create_directories(dest_file_dir);
            if (fs::copy_file(src_file_path, dest_file_path, copy_options)) {
                if (verbose_flag) {
                    fprintf(stdout, "%s -> %s\n", src_file_path.c_str(),
                            dest_file_path.c_str());
                }
            } else {
                if (verbose_flag && fs::exists(dest_file_path)) {
                    fprintf(stdout,
                            "Destination file %s already exists, skipped\n",
                            dest_file_path.c_str());
                } else {
                    fprintf(stderr, "Error: file %s could not be copied\n",
                            src_file_path.c_str());
                }
            }

            src_files.erase(src_file_path);
        }

        // std::cout << song_path << '\n';
        // fs::create_directories(song_path.substr(0,
        // song_path.rfind('/')));

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
