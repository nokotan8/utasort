#include <filesystem>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "FileData.hpp"

class PathBuilder {
  public:
    PathBuilder(std::string format_str);

    std::string build_path(std::filesystem::path file);
    std::string build_path(FileData file_data);

  private:
    std::string format_str;
    std::unique_ptr<PathBuilder> next_dir_builder = nullptr;
    std::vector<std::function<std::string(FileData &)>> get_tag_funcs;
};
