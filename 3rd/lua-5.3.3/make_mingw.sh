
mkdir -p build && cd build
cmake -G "MSYS Makefiles" ..
cd ..
cmake --build build --config Release

mkdir -p lib_mingw
cp build/liblua.a lib_mingw/liblua.a

rm -r build







