mkdir build
del /Q build\*
cd build
call %SDK_DIR%\bin\setenv.cmd /x86
cmake -G "Visual Studio 10" ..
cmake --build . --target Install --config Release
