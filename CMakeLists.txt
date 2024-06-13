cmake_minimum_required(VERSION 3.22)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(
	Tergine
	VERSION 1.0
	LANGUAGES C
)

include_directories(include)
find_package(Curses REQUIRED)
include_directories(Tergine ${CURSES_INCLUDE_DIRS})

add_executable(Tergine main.c src/draw.c)
target_link_libraries(Tergine ${CURSES_LIBRARIES})
