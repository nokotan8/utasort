#include "FileData.hpp"

#include <filesystem>
#include <string>

std::filesystem::path getFilePath(const FileData &fileData) {
    return fileData.file_path;
}

std::string getTitle(const FileData &fileData) { return fileData.title; }

std::string getAlbum(const FileData &fileData) { return fileData.album; }

std::string getArtist(const FileData &fileData) { return fileData.artist; }

std::string getAlbumArtist(const FileData &fileData) {
    return fileData.album_artist;
}

std::string getGenre(const FileData &fileData) { return fileData.genre; }

std::string getYear(const FileData &fileData) { return fileData.year; }

std::string getTrackNum(const FileData &fileData) { return fileData.track_number; }

