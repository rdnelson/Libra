call %SDK_DIR%\bin\SetEnv.cmd /Debug
cd msvc10
msbuild Libra.sln /nologo /verbosity:m /p:Configuration=Debug
call %SDK_DIR%\bin\SetEnv.cmd /Release
msbuild Libra.sln /nologo /verbosity:m /p:Configuration=Release
cd ..
bin\LibraTest.exe --gtest_output=xml
