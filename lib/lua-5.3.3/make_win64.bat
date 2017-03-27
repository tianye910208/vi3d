mkdir build & pushd build
cmake -G "Visual Studio 12 2013 Win64" ..
popd

cmake --build build --config Release

mkdir lib_win64
copy /Y build\Release\lua.lib lib_win64\lua.lib

rmdir /s /q build
pause