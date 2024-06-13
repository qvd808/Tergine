cmake_minimum_required(VERSION 3.22)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(
	Tergine
	VERSION 1.0
	LANGUAGES C
)

# Include the include directory
include_directories(include)

#Handle library
# find_library("ncursesw" NAME curses)
# if (EXISTS /usr/include/ncursesw/ncurses.h)
	# set(CURSES_USE_NCURSES TRUE)
# endif()

set(CURSES_NEED_WIDE TRUE)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

#Build an exuctable
add_executable(Tergine main.c src/draw.c)
target_link_libraries(Tergine ${CURSES_LIBRARIES})
