cmake_minimum_required(VERSION 3.5)
project(kiddgb)

add_compile_options(-std=c++14)

include_directories(ext/libelfin ext/linenoise)
add_executable(kidbg kidbg.cpp ext/linenoise/linenoise.c)

add_custom_target(
  libelfin
  COMMAND make
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/ext/libelfin
)

target_link_libraries(
  kidbg
  ${PROJECT_SOURCE_DIR}/ext/libelfin/dwarf/libdwarf++.so
  ${PROJECT_SOURCE_DIR}/ext/libelfin/elf/libelf++.so
)

add_dependencies(kidbg libelfin)

