Feature
-------
Can unpack / repack DATA.001 files and is compatible with all known retail game versions.

Download
--------
[Romhacking](https://www.romhacking.net/utilities/1716)

How to Use
----------
Using the executable:

* Unpacker: Expect DATA.001 and the PSX.EXE to be in the same directory as the executable and extract the files to a "DATA" directory.

* Repacker: Expect “DATA” directory containing game files and the PSX.EXE to be in the same directory as the executable and repack DATA.001 and the PSX.EXE to a “Repacked” directory.

With console arguments:

* Unpacker arguments: [0] [PSX.EXE path] [DATA.001 path] [Unpacked files path].

* Repacker arguments: [1] [PSX.EXE path] [Unpacked files path] [Repacked PSX.EXE path] [Repacked DATA.001 path].

Building
--------
Requirements:
* CMake
* C++20

Dependencies
-----------
* [fmt](https://github.com/fmtlib/fmt)
