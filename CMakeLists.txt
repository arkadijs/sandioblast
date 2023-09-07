cmake_minimum_required(VERSION 2.8)

project(sandioblast)
set(CMAKE_C_STANDARD 11)
add_executable(${PROJECT_NAME} main.c sandio.c evdev.c config.c decl.h)

find_package(PkgConfig REQUIRED)

pkg_check_modules(PC_EVDEV REQUIRED libevdev)
find_path(EVDEV_INC libevdev/libevdev.h REQUIRED HINTS ${PC_EVDEV_INCLUDE_DIRS})
find_library(EVDEV_LIB NAMES evdev libevdev REQUIRED HINTS ${PC_EVDEV_LIBRARY_DIRS})

target_compile_options(${PROJECT_NAME} PRIVATE -pedantic -Os -Wall -Wextra)
target_include_directories(${PROJECT_NAME} PRIVATE ${EVDEV_INC})
target_link_libraries(${PROJECT_NAME} ${EVDEV_LIB})
