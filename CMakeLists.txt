cmake_minimum_required(VERSION 3.10)
project(Weekendfun_RayTracing)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add this line to specify the include directories for the standard library
include_directories("/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1")

# Add source files
file(GLOB SOURCES "src/*.cc")
add_executable(WeekendfunRayTracing ${SOURCES})

# Specify the SDK path if needed
set(CMAKE_OSX_SYSROOT "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk")
