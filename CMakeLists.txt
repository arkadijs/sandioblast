cmake_minimum_required(VERSION 3.10)

project(sandioblast C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PC_EVDEV REQUIRED IMPORTED_TARGET libevdev)

add_executable(${PROJECT_NAME} main.c sandio.c evdev.c config.c decl.h)

target_compile_options(${PROJECT_NAME} PRIVATE -pedantic -Os -Wall -Wextra)
target_link_libraries(${PROJECT_NAME} PRIVATE PkgConfig::PC_EVDEV)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
