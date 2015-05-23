@echo off

mkdir msvc10
cd msvc10
%QTDIR%\bin\qmake -spec win32-msvc2010 -tp vc -r ..

IF "%1"=="" GOTO Release
IF "%1"=="release" GOTO Release

IF "%1"=="debug" GOTO Debug
IF "%1"=="all" GOTO Debug
GOTO BadArg

:Debug
mkdir ..\bin_debug
del /Q ..\bin_debug\*

call "%SDK_DIR%\bin\SetEnv.cmd" /x86 /Debug
msbuild src\src.vcxproj /nologo /verbosity:m /p:Configuration=Debug
msbuild doc\doc.vcxproj /nologo /verbosity:m /p:Configuration=Debug

xcopy /Y /E bin_debug\* ..\bin_debug\

IF NOT "%1"=="all" GOTO Done 

:Release
mkdir ..\bin
del /Q ..\bin\*

call "%SDK_DIR%\bin\SetEnv.cmd" /x86 /Release
msbuild src\src.vcxproj /nologo /verbosity:m /p:Configuration=Release
msbuild doc\doc.vcxproj /nologo /verbosity:m /p:Configuration=Release

xcopy /Y /E bin\* ..\bin\

:BadArg
echo "Invalid build type"
exit

:Done

