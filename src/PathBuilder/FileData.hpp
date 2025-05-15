#include <filesystem>
#include <functional>
#include <string>
#include <unordered_map>

// Metadata of a file
struct FileData {
    std::filesystem::path file_path;
    std::string title;
    std::string album;
    std::string artist;
    std::string album_artist;
    std::string genre;
    std::string year;
    std::string file_ext;
    std::string track_number;
};

std::string getTitle(const FileData &fileData);

std::string getAlbum(const FileData &fileData);

std::string getArtist(const FileData &fileData);

std::string getAlbumArtist(const FileData &fileData);

std::string getGenre(const FileData &fileData);

std::string getYear(const FileData &fileData);

std::string getTrackNum(const FileData &fileData);

// Map to convert tag string to function that gets the respective tag
const std::unordered_map<std::string, std::function<std::string(FileData &)>>
    STRING_TO_TAG_FN = {
        {"title", getTitle},
        {"artist", getArtist},
        {"album", getAlbum},
        {"albumartist", getAlbumArtist},
        {"genre", getGenre},
        {"year", getYear},
        {"tracknumber", getTrackNum}
    };
