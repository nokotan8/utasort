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
    TagLib::PropertyMap audio_file_props = audio_file.properties();
    if (audio_file.isNull()) {
        throw std::invalid_argument("Specified file does not exist");
    }

    std::string res_str = format_str;
    std::unordered_map<std::string, std::string> file_tags;

    for (std::string tag : format_tags) {
        if (file_tags.find(tag) == file_tags.end()) {
            std::string new_tag =
                (audio_file_props.find(tag) != audio_file_props.end())
                    ? audio_file_props[tag].toString().to8Bit(true)
                    : "";

            // Forward slashes not allowed in file path
            std::replace(new_tag.begin(), new_tag.end(), '/', '_');
            file_tags[tag] = new_tag;
        }
        std::size_t format_pos = res_str.find('\x01');
        res_str.replace(format_pos, 1, file_tags[tag]);
    }
    res_str += audio_file_path.extension();
    return res_str;
}
