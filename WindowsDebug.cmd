cd ..
if exist build rmdir /s /q build
if exist install rmdir /s /q install
mkdir build
conan install Test_surface -if build --build missing -s build_type=Debug -s compiler="Visual Studio" -s compiler.version=16 -s compiler.runtime="MDd" -r conancenter
cd build
cmake ../Test_surface -G "Visual Studio 16 2019" -A x64 -DCMAKE_BUILD_TYPE=Debug
cmake --build . --config Debug
cmake --install .
