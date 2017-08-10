export ANDROID_NDK=~/android-ndk-r10e

mkdir -p build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/android.toolchain.cmake -DANDROID_TOOLCHAIN_NAME=arm-linux-androideabi-clang3.6 -DANDROID_NATIVE_API_LEVEL=android-9 ../
cd ..
cmake --build build --config Release

mkdir -p lib_android/
cp build/liblua.a lib_android/liblua.a

rmdir -r build


