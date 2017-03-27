mkdir -p build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/iOS.cmake  -GXcode ../

cd ..
cmake --build build --config Release

mkdir lib_ios/
cp build/Release-iphoneos/liblua.a lib_ios/liblua.a 

rmdir -r build


