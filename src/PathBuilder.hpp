#include <filesystem>
#include <string>
#include <unordered_set>
#include <vector>

const int FS_EXT4 = 0;
const int FS_EXFAT = 1;

const std::unordered_set<char> invalid_chars_exfat = {'"', '*', '/',  ':', '<',
                                                      '>', '?', '\\', '|'};
class PathBuilder {
  public:
    PathBuilder(std::string format_str);

    std::string build_path(std::filesystem::path audio_file_path);

    static void replace_invalid_chars(std::string &str, char repl_char, int fs);

  private:
    std::string format_str;
    std::vector<std::string> format_tags;
};
