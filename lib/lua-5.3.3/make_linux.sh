
mkdir -p build && cd build
cmake ..
cd ..
cmake --build build --config Release

mkdir -p lib_linux
cp build/liblua.a lib_linux/liblua.a

rm -r build







