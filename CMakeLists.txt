if(UNIX)
    set(CMAKE_C_COMPILER winegcc)
    set(CMAKE_C_FLAGS "-mno-cygwin -mwindows")
endif()

cmake_minimum_required(VERSION 3.0)
project(TextViewer)

include_directories(include)
file(GLOB SRC src/*.c)

add_executable(TextViewer ${SRC} resources.rc)

if (WIN32)
    target_link_libraries(TextViewer gdi32.dll user32.dll kernel32.dll comctl32.dll comdlg32.dll)
endif()