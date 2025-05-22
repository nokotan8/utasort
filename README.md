# Utasort

Utasort is a command line program to organise audio files into a folder hierarchy based on their metadata.

> [!IMPORTANT]
> This project is a work in progress. I recommend making backups before using this program, and use the `--preview`
> tag to sanity check the expected result before writing anything to disk.

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

```
Usage: utasort -f <format_str> -s <source_dir> [options]...
Required:
  -s, --source path	Path to the source directory
  -f, --format string	Format string for the directory structure
Optional:
  -d, --dest path	Path to the destination directory;
			If not specified, the source directory will be used
  -t, --threads integer	The number of threads to use when copying files;
			The default is 1, and it is recommended to use no more than 8
  -r, --replace		Replace files in the destination directory if there are collisions;
			If not specified, existing files will not be overwritten
  -p, --preview		Show a list of files that would be moved/copied,;
			but do not commit any changes to disk
  -v, --verbose		Print more information, including the destination of all copied files
Help:
  -h, --help		Show this help message and exit
```

### Basic Usage

```bash
./utasort -f "{albumartist}/{album}/{tracknumber}-{title}" -s /path/to/music/src -d /path/to/dest
```

The `-f/--format` option is used to define the directory structure into which files will be sorted.
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

Depending on your files' tag format, they may have non-conventional tags that may or may not be
recognised by this program. Use the `-p/--preview` flag to check if unsure. See
[https://taglib.org/api/classTagLib_1_1PropertyMap.html](https://taglib.org/api/classTagLib_1_1PropertyMap.html)
for a list of common tags.

### Multithreading

This program supports using multiple threads to process and copy files concurrently.
However, since copying files is I/O rather than CPU intensive, the performance gains
from this are not mind blowing, though maybe noticeable depending on the speed
of your disk and the number/size of files being copied. Based on testing, I would
recommend using a maximum of 8 threads on an SSD, as the overhead of locking/unlocking
results in negligible performance improvements (or deterioration) after a certain point.
On a HDD, it is recommended to stick to 1 thread as concurrent writes may worsen
performance due to incurring more seek operations.

### Benchmarks

Benchmarks can be found in [benchmarks.md](/benchmarks.md)

#### TLDR

Fast on SSDs, with multithreading yielding considerable improvements. With bigger files,
multithreading yields smaller improvements.

Similar speed to other software on HDDs, with multithreading resulting in performance losses.

## Third-Party Software Notice

This software uses TagLib (dynamically linked) to read audio file metadata.

TagLib is licensed under the [GNU Lesser General Public License](https://www.gnu.org/licenses/lgpl-3.0.html) (LGPL)
and the [Mozilla Public License](https://www.mozilla.org/en-US/MPL/1.1/) (MPL).

You may obtain a copy of the TagLib source code from [https://taglib.org](https://taglib.org).

No modifications have been made to the TagLib library, and it is used in compliance with the LGPL terms.
