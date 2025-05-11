#include "PathBuilder.hpp"
#include <algorithm>
#include <cstddef>
#include <regex>
#include <stdexcept>
#include <string>

PathBuilder::PathBuilder(std::string str) {
    format_str = str;

    std::string next_str;
    const std::size_t curr_dir_end_index = format_str.find('/');
    if (curr_dir_end_index != format_str.npos) {
        next_str = format_str.substr(curr_dir_end_index + 1);
        format_str = format_str.substr(0, curr_dir_end_index + 1);
    }

    // Format strings are passed as {tag_name}. The regex matches 1+
    // non-whitespace and non-curly brace chars within curly braces
    std::regex tag_regex(R"(\{([^\{\}\s]+)\})");
    std::smatch match;
    while (std::regex_search(format_str, match, tag_regex)) {
        std::string tag = match[1];
        std::transform(tag.begin(), tag.end(), tag.begin(), ::tolower);
        if (STRING_TO_TAG_FN.find(tag) != STRING_TO_TAG_FN.end()) {
            get_tag_funcs.push_back(STRING_TO_TAG_FN.at(tag));
            format_str.replace(match.position(), match.length(), "{}");
        } else {
            throw std::invalid_argument("Specified tag '" + tag +
                                        "' is not valid");
        }
    }

    if (next_str.length()) {
        next_dir_builder = new PathBuilder(next_str);
    } else {
        next_dir_builder = nullptr;
    }
}

// std::string PathBuilder::build_path(std::filesystem::path file) {}

std::string PathBuilder::build_path(FileData file_data) {
    std::string res_str = format_str;
    for (auto &f : get_tag_funcs) {
        std::size_t format_pos = res_str.find("{}");
        res_str.replace(format_pos, 2, f(file_data));
    }

    if (next_dir_builder == nullptr) {
        return res_str + '.' + file_data.file_ext;
    } else {
        return res_str + next_dir_builder->build_path(file_data);
    }
}
