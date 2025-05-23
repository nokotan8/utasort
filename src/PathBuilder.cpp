#include "PathBuilder.hpp"
#include <algorithm>
#include <cctype>
#include <regex>
#include <stdexcept>
#include <string>
#include <taglib/fileref.h>
#include <taglib/tpropertymap.h>
#include <unordered_map>

PathBuilder::PathBuilder(std::string str) {
    format_str = str;

    // Format strings are passed as {tag_name}. The regex matches 1+
    // non-whitespace and non-curly brace chars within curly braces
    std::regex tag_regex(R"(\{([^\{\}\s]+)\})");
    std::smatch match;
    while (std::regex_search(format_str, match, tag_regex)) {
        std::string tag = match[1];
        std::transform(tag.begin(), tag.end(), tag.begin(), ::toupper);
        format_tags.push_back(tag);
        format_str.replace(match.position(), match.length(), "\x01");
    }
}

std::string PathBuilder::build_path(std::filesystem::path audio_file_path) {
    TagLib::FileRef audio_file(audio_file_path.c_str());
    if (audio_file.isNull()) {
        throw std::invalid_argument(
            "Specified file does not exist or is not a supported audio file");
    }
    TagLib::PropertyMap audio_file_props = audio_file.properties();

    std::string res_str = format_str;
    std::unordered_map<std::string, std::string> file_tags;

    // for (const auto &tag : audio_file_props) {
    //     std::cout << tag.first << ' ' << tag.second.toString().to8Bit(true) << '\n';
    // }

    for (std::string tag : format_tags) {
        if (file_tags.find(tag) == file_tags.end()) {
            std::string new_tag =
                (audio_file_props.find(tag) != audio_file_props.end())
                    ? audio_file_props[tag].toString().to8Bit(true)
                    : "";

            replace_invalid_chars(new_tag, '_', FS_EXFAT);
            file_tags[tag] = new_tag;
        }
        std::size_t format_pos = res_str.find('\x01');
        res_str.replace(format_pos, 1, file_tags[tag]);
    }
    res_str += audio_file_path.extension();
    return res_str;
}

void PathBuilder::replace_invalid_chars(std::string &str, char repl_char,
                                        int fs) {
    for (int i = 0; i < str.size(); i++) {
        if (invalid_chars_exfat.count(str[i])) {
            str[i] = repl_char;
        }
    }
}
