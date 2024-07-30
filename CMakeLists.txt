cmake_minimum_required(VERSION 3.22)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(
    Tergine
    VERSION 1.0
    LANGUAGES C
)
#Set option for profilling
option(PROFILE "Enable profiling with gprof" OFF)

if (PROFILE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg -g")
    message(STATUS "Profiling is enabled")
endif()

# Include the include directory
include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)

#Handle library
# find_library("ncursesw" NAME curses)
# if (EXISTS /usr/include/ncursesw/ncurses.h)
    # set(CURSES_USE_NCURSES TRUE)
# endif()

#Include NCURSE Package
set(CURSES_NEED_WIDE TRUE)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

#Include the MATH LIBRARY
find_library(MATH_LIBRARY m REQUIRED)

#Build an exuctable
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -g2")
add_executable(Tergine main.c lib/draw.c lib/setup.c lib/util.c)

#Linked Library
target_link_libraries(Tergine ${CURSES_LIBRARIES} ${MATH_LIBRARY})
