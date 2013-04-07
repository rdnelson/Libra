call %SDK_DIR%\bin\SetEnv.cmd
cd msvc10
msbuild Libra.sln /nologo /verbosity:m
cd ..
bin\LibraTest.exe --gtest_output=xml
