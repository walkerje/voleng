# This file is responsible for finding and adding dependencies to the list of linked libraries.
# At the moment, it assumes all dependencies are covered either by built-in CMake modules
# or that the developer has otherwise gone through the cmake/make/install process.

find_package(X11 REQUIRED)      # Covered in CMake modules...
find_package(OpenGL REQUIRED)   # Covered in CMake modules...
find_package(PhysFS REQUIRED)   # Covered in CMake modules...
find_package(Threads REQUIRED)  # Covered in CMake modules...

find_package(glfw3 REQUIRED)    # Must build from source & install
find_package(SoLoud REQUIRED)   # Must build from source & install

find_package(Poco REQUIRED Foundation Util JSON )

set(POCO_LINKED_LIBS
        Poco::Foundation
        Poco::Util
        Poco::JSON)

set(LINKED_LIBRARY_HEADERS
        ${GLFW_INLUDE_DIRS})

set(LINKED_LIBRARIES
        dl
        ${CMAKE_THREAD_LIBS_INIT}
        ${PHYSFS_LIBRARY}
        GL
        glfw
        soloud
        ${X11_X11_LIB}
        ${POCO_LINKED_LIBS})