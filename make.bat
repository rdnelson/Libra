mkdir bin
del /Q bin\*
call %SDK_DIR%\bin\SetEnv.cmd /x86 /Debug
cd msvc10
msbuild Libra.sln /nologo /verbosity:m /p:Configuration=Debug
call %SDK_DIR%\bin\SetEnv.cmd /x86 /Release
msbuild Libra.sln /nologo /verbosity:m /p:Configuration=Release
cd ..
bin\LibraTest.exe --gtest_output=xml
