#include <filesystem>
#include <string>
#include <vector>

class PathBuilder {
  public:
    PathBuilder(std::string format_str);

    std::string build_path(std::filesystem::path audio_file_path);

  private:
    std::string format_str;
    std::vector<std::string> format_tags;
};
