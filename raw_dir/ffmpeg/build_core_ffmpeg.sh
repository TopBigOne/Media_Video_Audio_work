#!/bin/bash
# 以下路径需要修改成自己的NDK目录
TOOLCHAIN=/Users/dev/Documents/Android_Env/Sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64
# 最低支持的android sdk版本
API=21

function build_android {
  echo "================start compile ffmpeg for: $CPU ======================"
  ./configure \
    --prefix=$PREFIX \
    --disable-hwaccels \
    --disable-gpl \
    --disable-postproc \
    --enable-shared \
    --enable-jni \
    --disable-mediacodec \
    --enable-small \
    --enable-gpl \
    --disable-decoder=h264_mediacodec \
    --disable-static \
    --disable-doc \
    --disable-programs \
    --disable-ffmpeg \
    --disable-ffplay \
    --disable-ffprobe \
    --disable-avdevice \
    --disable-symver \
    --enable-neon \
    --disable-asm \
    --cross-prefix=$CROSS_PREFIX \
    --target-os=android \
    --arch=$ARCH \
    --cpu=$CPU \
    --cc=$CC \
    --cxx=$CXX \
    --enable-cross-compile \
    --sysroot=$SYSROOT \
    --extra-cflags="-mno-stackrealign -Os $OPTIMIZE_CFLAGS -fPIC" \
    --extra-ldflags="$ADDI_LDFLAGS" \
    $ADDITIONAL_CONFIGURE_FLAG
  make clean
  make -j16
  make install
  echo "================The Compilation of ffmpeg for $CPU is completed================"
}

# armv8-a
ARCH=arm64
CPU=armv8-a
# r21版本的ndk中所有的编译器都在/toolchains/llvm/prebuilt/darwin-x86_64/目录下（clang）
CC=$TOOLCHAIN/bin/aarch64-linux-android$API-clang
CXX=$TOOLCHAIN/bin/aarch64-linux-android$API-clang++
# NDK头文件环境
SYSROOT=$TOOLCHAIN/sysroot
CROSS_PREFIX=$TOOLCHAIN/bin/aarch64-linux-android-
# so输出路径
PREFIX=$(pwd)/android_so_result/arm64-v8a
OPTIMIZE_CFLAGS="-march=$CPU"
build_android

# 交叉编译工具目录,对应关系如下
# armv8a -> arm64 -> aarch64-linux-android-
# armv7a -> arm -> arm-linux-androideabi-
# x86 -> x86 -> i686-linux-android-
# x86_64 -> x86_64 -> x86_64-linux-android-

# CPU架构
# armv7-a
ARCH=arm
CPU=armv7-a
CC=$TOOLCHAIN/bin/armv7a-linux-androideabi$API-clang
CXX=$TOOLCHAIN/bin/armv7a-linux-androideabi$API-clang++
SYSROOT=$TOOLCHAIN/sysroot
CROSS_PREFIX=$TOOLCHAIN/bin/arm-linux-androideabi-
PREFIX=$(pwd)/android_so_result/armeabi-v7a
OPTIMIZE_CFLAGS="-mfloat-abi=softfp -mfpu=vfp -marm -march=$CPU "
build_android

# x86
ARCH=x86
CPU=x86
CC=$TOOLCHAIN/bin/i686-linux-android$API-clang
CXX=$TOOLCHAIN/bin/i686-linux-android$API-clang++
SYSROOT=$TOOLCHAIN/sysroot
CROSS_PREFIX=$TOOLCHAIN/bin/i686-linux-android-
PREFIX=$(pwd)/android_so_result/$CPU
OPTIMIZE_CFLAGS="-march=i686 -mtune=intel -mssse3 -mfpmath=sse -m32"
build_android

# x86_64
ARCH=x86_64
CPU=x86-64
CC=$TOOLCHAIN/bin/x86_64-linux-android$API-clang
CXX=$TOOLCHAIN/bin/x86_64-linux-android$API-clang++
SYSROOT=$TOOLCHAIN/sysroot
CROSS_PREFIX=$TOOLCHAIN/bin/x86_64-linux-android-
PREFIX=$(pwd)/android_so_result/x86_64
OPTIMIZE_CFLAGS="-march=$CPU -msse4.2 -mpopcnt -m64 -mtune=intel"
# 方法调用
build_android
