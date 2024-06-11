cmake_minimum_required(VERSION 3.22)
project(
	Tergine
	VERSION 1.0
	LANGUAGES C
)

find_package(Curses REQUIRED)
include_directories(Tergine ${CURSES_INCLUDE_DIRS})

add_executable(Tergine main.c)
target_link_libraries(Tergine ${CURSES_LIBRARIES})
