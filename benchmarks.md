# Benchmarks

In these benchmarks, the only condition to be considered successful is for the files to be
copied to the new directory without any changes to its binary data or metadata. Other software
may have slight differences in how tags and invalid characters are handled and as a result,
slight differences in file/directory names. As long as the result roughly follows the given format,
it is considered successful. I did not modify any third-party software used, so if they were doing
other things that took up processing power, that would also be included in the run time.

These are just some quick and dirty measurements, averaged over 1-3 runs using my personal music files
and the hardware I have lying around. They are just for fun; take them with a grain of salt. They are
not for me to boast that my software is better than everyone else's; if you're keeping audio files around in this day
and age, pure speed is probably not what you value most.

Unless mentioned otherwise, measurements are made with GNU `time`.

## SSD to SSD

**SSD model:** SK hynix Platinum P41 PCIe NVMe Gen4 M.2 Internal SSD

#### AAC files

| Description | Command | Time (s) |
| -------------------------------------------------------------- | --------------------------------------------------------------------------------------------------- | -------- |
| Utasort w/ 1162 Apple AAC (.m4a) files (9.3GB), 1 thread | `./utasort -s ~/shared/Music/Portable -d ~/Music/Library -f '{tracknumber} {title}' -t 1 --verbose` | 4.400 |
| Utasort w/ 1162 .m4a files, 2 threads | `./utasort -s ~/shared/Music/Portable -d ~/Music/Library -f '{tracknumber} {title}' -t 2 --verbose` | 3.502 |
| Utasort w/ 1162 .m4a files, 4 threads | `./utasort -s ~/shared/Music/Portable -d ~/Music/Library -f '{tracknumber} {title}' -t 4 --verbose` | 2.708 |
| Utasort w/ 1162 .m4a files, 6 threads | `./utasort -s ~/shared/Music/Portable -d ~/Music/Library -f '{tracknumber} {title}' -t 6 --verbose` | 3.460 |
| Utasort w/ 1162 .m4a files, 8 threads | `./utasort -s ~/shared/Music/Portable -d ~/Music/Library -f '{tracknumber} {title}' -t 8 --verbose` | 2.746 |
| Other Open-source Python software w/ 1162 .m4a files, 1 thread | - | 12.576 |

#### FLAC files

| Description | Command | Time (s) |
| --------------------------------------------------------------- | --------------------------------------------------------------------------------------------------- | -------- |
| Utasort w/ 1162 FLAC (.flac) files (26.9GB), 1 thread | `./utasort -s ~/shared/Music/Original -d ~/Music/Library -f '{tracknumber} {title}' -t 1 --verbose` | 22.331 |
| Utasort w/ 1162 .flac files, 2 thread | `./utasort -s ~/shared/Music/Original -d ~/Music/Library -f '{tracknumber} {title}' -t 2 --verbose` | 16.422 |
| Utasort w/ 1162 .flac files, 4 thread | `./utasort -s ~/shared/Music/Original -d ~/Music/Library -f '{tracknumber} {title}' -t 4 --verbose` | 16.180 |
| Other Open-source Python software w/ 1162 .flac files, 1 thread | - | 33.109 |

## SSD to HDD

**SSD model:** SK hynix Platinum P41 PCIe NVMe Gen4 M.2 Internal SSD

**HDD model:** WD Passport Portable HDD (I don't know the exact model, but it's old)

#### AAC files

| Description | Command | Time (s) |
| -------------------------------------------------------------- | --------------------------------------------------------------------------------------------------- | -------- |
| Utasort w/ 1162 Apple AAC (.m4a) files (9.3GB), 1 thread | `./utasort -s ~/shared/Music/Portable -d ~/media/Library -f '{tracknumber} {title}' -t 1 --verbose` | 115.770 |
| Utasort w/ 1162 .m4a files, 2 threads | `./utasort -s ~/shared/Music/Portable -d ~/media/Library -f '{tracknumber} {title}' -t 2 --verbose` | 231.000 |
| Other Open-source Python software w/ 1162 .m4a files, 1 thread | - | 118.750 |


<!-- #### FLAC files -->
<!---->
<!-- | Description | Command | Time (s) | -->
<!-- | --------------------------------------------------------------- | --------------------------------------------------------------------------------------------------- | -------- | -->
<!-- | Utasort w/ 1162 FLAC (.flac) files (26.9GB), 1 thread | `./utasort -s ~/shared/Music/Original -d ~/media/Library -f '{tracknumber} {title}' -t 1 --verbose` | | -->
<!-- | Other Open-source Python software w/ 1162 .flac files, 1 thread | - | | -->

## Conclusion

Once again, 
take these results with a grain of salt. This SSD is really fast, and this HDD is really slow but it's the only
one I have lying around. You may get different results using your own files and hardware.

Utasort can be considerably more performant than other similar software when reading/writing on an SSD, even when
comparing singlethreaded performance. However, performance is roughly equal when tested on a HDD. 

Multithreading, especially up to 4 threads, can have significant improvements when reading/writing smaller files
on an SSD, as fast disk speeds mean that the program is both CPU and I/O bound. However when it comes
to reading larger files such as `.flac` audio files, the program becomes significantly I/O bound and additional
threads just end up waiting around for throughput to become available.

As expected, since concurrent writes to a HDD cause require the physical read/write heads to move around, it is less efficient than
sequential writing. 
