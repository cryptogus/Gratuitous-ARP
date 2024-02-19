cmake_minimum_required(VERSION 3.10)
project(send-garp LANGUAGES C)

option(TOOLCHAIN OFF)
if(TOOLCHAIN)
    execute_process(
        COMMAND bash -c "export STAGING_DIR=/home/cryptogus/openwrt-2102-Oolite-MT7981/staging_dir"
    )
    SET(CMAKE_C_COMPILER "/home/cryptogus/openwrt-2102-Oolite-MT7981/staging_dir/toolchain-aarch64_cortex-a53_gcc-8.4.0_musl/bin/aarch64-openwrt-linux-musl-gcc")
endif()
include_directories(src)

add_executable(${CMAKE_PROJECT_NAME} src/garp.c src/main.c)