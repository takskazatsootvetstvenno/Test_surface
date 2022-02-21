#!/bin/bash
sudo pip install conan
sudo apt-get -y install cmake
sudo apt-get install libgl1-mesa-dev
sudo apt-get install libglu1-mesa-dev
cd ..
rm -r build
rm -r install
mkdir build
conan install Test_surface -if build --build missing -s build_type=Release -r conancenter
cd build
cmake ../Test_surface -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
cmake --install .
