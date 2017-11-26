#### Components 
### _A collection of generic programming tools_

## Synopsis
This repository is a collection of generic programming tools not specific to any
one single code project. It exists as part of an effor to aid the C++ code bases 
of Komorebi Games.

## Reuired Software
This software is designed with the intent to run on a Unix/Linux environment. Certain aspects may be able to compile for Windows, but there is no guartantee it will. 

Building requires at least:
  __cmake v3.5.0__
  __gcc 4.6__
  __c++11__
  _access to Google Test Repository_

## Build & Installation
To build this repository perform an out-of-source build. This means we will compile the code in a directory tha is not part of the source files. Steps in 
*_this font_* are to be typed verbatim on the terminal.
Steps:
 
0. *_cd _*_[top level where this file is]_*
1. *_cd build_*_[or where ever you plan to build]
2. *_cmake .._* _[any cmake defines]_
3. *_make -j12_* _[or *-j32* if you're real fancy]_
4. You're done! You can now use the libComponents.so as part of your build
   and use the includes.

## License

This file is part of Components.

    Components is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Components is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Components.  If not, see <http://www.gnu.org/licenses/>