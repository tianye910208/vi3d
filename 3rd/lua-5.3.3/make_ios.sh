mkdir -p lib_ios/
mkdir -p lib_ios/Release
mkdir -p lib_ios/Debug

mkdir -p build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/iOS.cmake -GXcode ../

cd ..
cmake --build build --config Release
cmake --build build --config Debug 

cp build/Release-iphoneos/liblua.a lib_ios/Release/liblua-os.a 
cp build/Debug-iphoneos/liblua.a lib_ios/Debug/liblua-os.a 
rm -r build


mkdir -p build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/iOS.cmake -DIOS_PLATFORM=SIMULATOR  -GXcode ../

cd ..
cmake --build build --config Release
cmake --build build --config Debug 

cp build/Release-iphonesimulator/liblua.a lib_ios/Release/liblua-simulator.a 
cp build/Debug-iphonesimulator/liblua.a lib_ios/Debug/libluad-simulator.a 
rm -r build

lipo -create lib_ios/Release/liblua-os.a lib_ios/Release/liblua-simulator.a -output lib_ios/Release/liblua.a
lipo -create lib_ios/Debug/liblua-os.a lib_ios/Debug/libluad-simulator.a -output lib_ios/Debug/liblua.a



