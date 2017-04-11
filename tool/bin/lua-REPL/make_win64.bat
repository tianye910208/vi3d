@echo off

if exist "%VS120COMNTOOLS%" (
set VCVARS="%VS120COMNTOOLS%..\..\VC\vcvarsall.bat"
goto build
) else (
goto missing
)

:build

call %VCVARS% x64



mkdir build_win64

set CCFLAG=-I..\..\..\3rd\lua-5.3.3\src

cl -c -nologo -EHsc -D_CRT_SECURE_NO_WARNINGS  %CCFLAG%  -Fobuild_win64\lua.o lua.c
cl -c -nologo -EHsc -D_CRT_SECURE_NO_WARNINGS  %CCFLAG%  -Fobuild_win64\lfs.o lfs.c
cl -c -nologo -EHsc -D_CRT_SECURE_NO_WARNINGS  %CCFLAG%  -Fobuild_win64\liconv.o liconv.c

link -nologo -machine:x64 -out:..\lua.exe -libpath:..\..\..\3rd\lua-5.3.3\lib_win64\ lua.lib  build_win64\lua.o build_win64\lfs.o build_win64\liconv.o
rmdir /s /q build_win64


goto waiting

:missing
echo Can't find Visual Studio 2013.


:waiting
pause







