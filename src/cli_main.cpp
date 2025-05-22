#include "copy_files.hpp"
#include "flags.hpp"
#include "get_files.hpp"

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <getopt.h>
#include <iostream>
#include <string>
#include <unordered_set>

namespace fs = std::filesystem;

int verbose_flag = 0;
int replace_flag = 0;
int move_flag = 0;
int preview_flag = 0;

int main(int argc, char *argv[]) {
    std::string src_dir;
    std::string dest_dir;
    std::string format_str;
    int num_threads = 1;

    static struct option long_opts[] = {
        {"verbose", no_argument, 0, 'v'},
        {"help", no_argument, 0, 'h'},
        {"source", required_argument, 0, 's'},
        {"dest", required_argument, 0, 'd'},
        {"format", required_argument, 0, 'f'},
        {"threads", required_argument, 0, 't'},
        {"replace", no_argument, 0, 'r'},
        {"move", no_argument, 0, 'm'},
        {"preview", no_argument, 0, 'p'},
        {0, 0, 0, 0}

    };
    int opt_char;
    while (1) {

        int opt_index = 0;
        opt_char =
            getopt_long(argc, argv, "vhs:d:f:t:rmp", long_opts, &opt_index);

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
                std::cout << "  -d, --dest path\tPath to the destination "
                    "directory;\n";
                std::cout << "\t\t\tIf not specified, the source directory will be "
                    "used\n";
                std::cout << "  -t, --threads integer\tThe number of threads to "
                    "use when copying files;\n";
                std::cout << "\t\t\tThe default is 1, and it is recommended to use "
                    "no more than 8\n";
                std::cout << "  -r, --replace\t\tReplace files in the destination "
                    "directory if there are collisions;\n";
                std::cout << "\t\t\tIf not specified, existing files will not be "
                    "overwritten\n";
                std::cout << "  -p, --preview\t\tShow a list of files that would "
                    "be moved/copied,;\n";
                std::cout << "\t\t\tbut do not commit any changes to disk\n";
                std::cout << "  -v, --verbose\t\tPrint more information, including "
                    "the destination of all copied files\n";
                std::cout << "Help:\n";
                std::cout << "  -h, --help\t\tShow this help message and exit\n";
                std::cout << "  -H, --format-help\tShow help for defining the "
                    "format string and exit (doesn't work yet)\n";

                return 0;
            }
            case 'v':
                verbose_flag = 1;
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
                    fprintf(stderr, "Destination directory '%s' does not exist",
                            optarg);
                    return 1;
                }
                break;
            }
            case 't':
                num_threads = atoi(optarg);
                break;
            case 'f':
                format_str = optarg;
                break;
            case 'r':
                replace_flag = 1;
                break;
            case 'm':
                move_flag = 1;
                break;
            case 'p':
                preview_flag = 1;
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

    copy_files(src_files, format_str, dest_dir, num_threads);

    return 0;
}
