# Utasort

Utasort is a command line program to organise audio files into a folder hierarchy based on their metadata.

## Dependencies

- C++17 compiler (or newer)
- [TagLib](https://taglib.org/)
  - If you have VLC or most Linux distros, it's probably already installed

## Build Instructions

This project uses CMake (minimum required version 3.10).

1. **Create a build directory and navigate to it:**

```bash
mkdir build && cd build
```

2. **Configure:**

```bash
cmake ..
```

3. **Build:**

```bash
cmake --build .
```

## Usage

For detailed help instructions, build the project and run:

```bash
./utasort --help
```

**NOTE:** this is currently incomplete.

### Basic Usage

```bash
./utasort --format "{albumartist}/{album}/{tracknumber}-{title}" --source /path/to/music/src --dest /path/to/dest
```

The `--format` option is used to define the directory structure into which files will be sorted.
Fields to be replaced by the specific file's tags are contained within curly braces (`{}`).
Using the above example, files will be sorted by album artist followed by album, and renamed
to the track number and title separated by a hyphen. Some basic tags (case-insensitive) include:

- TITLE
- ALBUM
- ARTIST
- ALBUMARTIST
- TRACKNUMBER
- DISCNUMBER
- DATE
- GENRE
- COMMENT

As this uses the `Taglib::PropertyMap` class under the hood, refer to
[this document](https://taglib.org/api/classTagLib_1_1PropertyMap.html) for a full list of supported tags.

## Third-Party Software Notice

This software uses TagLib to read audio metadata.

TagLib is licensed under the [GNU Lesser General Public License](https://www.gnu.org/licenses/lgpl-3.0.html) (LGPL)
and the [Mozilla Public License](https://www.mozilla.org/en-US/MPL/1.1/) (MPL).

You may obtain a copy of the TagLib source code from:

[https://taglib.org](https://taglib.org)

No modifications have been made to the TagLib library, and it is used in compliance with the LGPL terms.
