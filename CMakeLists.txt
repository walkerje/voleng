cmake_minimum_required(VERSION 3.15)
project(VolEng)
set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Set compiler flag options.
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -Og") # Debug-ready compiler optimization.
set(CMAKE_CXX_FLAGS_RELEASE "-O3") # High compiler optimization. Longer compile times as a result.

include(cmake/Dependencies.cmake) # Include our dependencies here...

include_directories(include ${LINKED_LIBRARY_HEADERS}) # Set our primary project include directory

add_subdirectory(src)
add_subdirectory(src/gfx)
add_subdirectory(src/audio)

add_executable(VolEng ${SOURCE_FILES}) # specified in src/CMakeLists.txt
target_link_libraries(VolEng ${LINKED_LIBRARIES}) # specified in cmake/Dependencies.cmake
