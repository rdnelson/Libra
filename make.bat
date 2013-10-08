mkdir build
del /Q build\*
cd build
cmake -G "Visual Studio 10" ..
cmake --build . --target Install.vcxproj --config Release
