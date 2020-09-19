ProjectDecima is aimed to provide GUI for export/preview files inside Decima engine manager.

![](https://i.imgur.com/cEQnB4K.png)

## Project is heavily inspired by:
* [DecimaExplorer](https://github.com/Jayveer/Decima-Explorer) by [Jayveer](https://github.com/Jayveer)
* [DecimaTools](https://github.com/Wunkolo/DecimaTools) by [Wunkolo](https://github.com/Wunkolo)
* [ProjectZeroDawn](https://github.com/yretenai/ProjectZeroDawn) by [yretenai](https://github.com/yretenai)

## Requirements
* Windows 10 64-bit
* CMake 3.15 <=
* Any of listed compilers:
  * Visual Studio 2019
  * Clang 
  * MinGW x64

## Building
**Note: This project supports x64 Windows 10 systems only.**

Open console and run following commands:
1. ```git clone --recursive https://github.com/REDxEYE/ProjectDecima.git```
1. ```cd ProjectDecima```
1. ```git checkout development``` to get all latest features *(Optional)*
1. ```cmake CMakeLists.txt -G "Visual Studio 16 2019" -B build```
1. ```cmake --build build --config Release```

## License

* [Murmur3 implementation by PeterScott](https://github.com/PeterScott/murmur3/blob/master/murmur3.c)
* [Modified MD5 implementation by pod32g](https://github.com/pod32g/MD5/blob/master/md5.c)
* [mio Copyright (c) 2018 https://github.com/mandreyel/](https://github.com/mandreyel/mio)
