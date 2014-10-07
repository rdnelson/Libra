@echo off

mkdir bin
mkdir msvc10
del /Q bin\*
cd msvc10
%QTDIR%\bin\qmake -spec win32-msvc2010 -tp vc -r ..

call "%SDK_DIR%\bin\SetEnv.cmd" /x86 /Debug
msbuild src\src.vcxproj /nologo /verbosity:m /p:Configuration=Debug
msbuild doc\doc.vcxproj /nologo /verbosity:m /p:Configuration=Debug


call "%SDK_DIR%\bin\SetEnv.cmd" /x86 /Release
msbuild src\src.vcxproj /nologo /verbosity:m /p:Configuration=Release
msbuild doc\doc.vcxproj /nologo /verbosity:m /p:Configuration=Release

xcopy /Y /E bin\* ..\bin\
