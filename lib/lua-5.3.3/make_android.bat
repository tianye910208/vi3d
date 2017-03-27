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

mkdir build & pushd build

cmake .. -G "NMake Makefiles" -DCMAKE_TOOLCHAIN_FILE=..\cmake\android.toolchain.cmake -DANDROID_TOOLCHAIN_NAME=arm-linux-androideabi-clang3.6 -DANDROID_NATIVE_API_LEVEL=android-9

nmake
popd


mkdir lib_android\

copy /Y build\liblua.a lib_android\liblua.a

rmdir /s /q build



goto waiting

:missing
echo Can't find Visual Studio 2013.


:waiting
pause
