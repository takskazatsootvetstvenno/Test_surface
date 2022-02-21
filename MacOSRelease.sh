#!/bin/bash
sudo pip install conan
brew  -y install cmake
brew  install libgl1-mesa-dev
brew  install libglu1-mesa-dev
cd ..
rm -r build
rm -r install
mkdir build
conan install Test_surface -if build --build missing -s build_type=Release -r conancenter
cd build
cmake ../Test_surface -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
cmake --install .
