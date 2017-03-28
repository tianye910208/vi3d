@echo off

if exist "%VS120COMNTOOLS%" (
set VCVARS="%VS120COMNTOOLS%..\..\VC\vcvarsall.bat"
goto build
) else (
goto missing
)


:build

call %VCVARS% 

set ANDROID_NDK=D:\android\android-ndk-r10e

for %%i in (armeabi-v7a, armeabi, arm64-v8a, mips, mips64, x86, x86_64) do (

mkdir build & pushd build
cmake .. -G "NMake Makefiles" -DANDROID_ABI=%%i -DCMAKE_TOOLCHAIN_FILE=..\cmake\android.toolchain.cmake > ttt.txt
nmake
popd

mkdir lib_android\%%i
copy /Y build\liblua.a lib_android\%%i\liblua.a
rmdir /s /q build

)

goto waiting

:missing
echo Can't find Visual Studio 2013.


:waiting
pause

