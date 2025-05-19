#include <filesystem>
#include <string>
#include <unordered_set>

// Covers most audio file extensions
const std::unordered_set<std::string> valid_exts = {
    ".mp3", ".mpc", ".flac", ".mp4", ".asf", ".aiff", ".wav", ".dsf",
    ".dff", ".tta", ".wv",   ".ogg", ".oga", ".spx",  ".opus"};

int get_files(std::filesystem::path source_dir,
              std::unordered_set<std::filesystem::path> &src_files);
