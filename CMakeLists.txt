cmake_minimum_required(VERSION 3.12)

set(BUILD_NUMBER "0")

# Pull in SDKs (must be before project)
include(pico_sdk_import.cmake)
include(FreeRTOS_Kernel_import.cmake)


project(pico-freertos-simplest C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})

# Initialize the SDK
pico_sdk_init()

if ( "${CMAKE_BUILD_TYPE}" STREQUAL "Debug" )
add_compile_definitions(BUILD_TYPE_DEBUG=1)
elseif ( "${CMAKE_BUILD_TYPE}" STREQUAL "Release" )
add_compile_definitions(BUILD_TYPE_RELEASE=1)
else ()
message( FATAL_ERROR "BUILD_TYPE should be Debug or Release, '${CMAKE_BUILD_TYPE}' is not understood" )
endif ( )

add_subdirectory(blink)
add_subdirectory(hello_usb)
add_subdirectory(hello_rtos)

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        )
if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wno-maybe-uninitialized)
endif()

