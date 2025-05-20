cmake_minimum_required(VERSION 2.6.0)
    
# here we specify that the project is C language only, so the default
# C compiler on the system will be used
project(replay_trace LANGUAGES C CXX)


add_subdirectory(snmalloc EXCLUDE_FROM_ALL)
add_executable(replay_trace include/tlsf.h include/pmparser.h  replay_trace.c  alloc1.cc alloc2.cc alloc3.cc alloc4.cc alloc5.cc alloc6.cc alloc7.cc alloc8.cc alloc9.cc alloc10.cc alloc11.cc alloc12.cc alloc13.cc tlsf.c)

target_include_directories(replay_trace PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include ${CMAKE_CURRENT_LIST_DIR}/snmalloc/src)
set(SNMALLOC_HEADER_ONLY_LIBRARY ON)
set(SNMALLOC_ENABLE_WAIT_ON_ADDRESS ON)
SET (CMAKE_C_COMPILER  "/usr/bin/clang")
SET (CMAKE_CXX_COMPILER "/usr/bin/clang++")
SET (CMAKE_CXX_FLAGS "-std=c++17 -mcx16")

