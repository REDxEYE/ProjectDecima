This project is aimed to provide graphical user interface for previewing, exporting and modifying game resources in research purposes.

![The program interface showing texture of Sam's face](https://i.imgur.com/0yv9GsU.png)

## People who made this project possible
* [Wunkolo](https://github.com/Wunkolo) who reverse-engineered the format of game archives and extracted keys required for decryption
* [Yretenai](https://github.com/yretenai) who at early steps gave basic understanding how game works
* [Nukem9](https://github.com/Nukem9) who from nowhere kicked out the door and provided priceless intelligence about RTTI data that game uses to load its resources 

## Building
### Requirements:
* Windows 10 64-bit _(note: this is the only supported platform)_
* CMake 3.10 or greater
* Any of listed compilers:
  * Visual Studio 2019
  * Clang 
  * MinGW x64

### Steps:
Open console and run following commands:
1. ```git clone --recursive https://github.com/REDxEYE/ProjectDecima.git```
1. ```cd ProjectDecima```
1. ```git checkout development``` to get all latest features *(Optional)*
1. ```cmake CMakeLists.txt -G "Visual Studio 16 2019" -B build```
1. ```cmake --build build --config Release```

## Copyright
* [Library 'imgui'](https://github.com/ocornut/imgui) by [ocornut](https://github.com/ocornut)
* [Library 'mio'](https://github.com/mandreyel/mio) by [mandreyel](https://github.com/mandreyel)
* All rights to the Decima Engine, its source code and all games based on this engine belong to their developers.