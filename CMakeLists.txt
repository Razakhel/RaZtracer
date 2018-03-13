cmake_minimum_required(VERSION 3.6)
project(RaZtracer)

set(CMAKE_CXX_STANDARD 14)

if (CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-value -Wno-unused-parameter")
elseif (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif ()

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/extern
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/png
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/glew/include
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/glfw/deps
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/glfw/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

AUX_SOURCE_DIRECTORY(extern/png PNG_SRC)
AUX_SOURCE_DIRECTORY(extern/glew/src GLEW_SRC)
AUX_SOURCE_DIRECTORY(extern/glfw/src GLFW_SRC)

set(
    SRC

    ${PNG_SRC}
    ${GLEW_SRC}
    ${GLFW_SRC}

    src/RaZtracer/Math/*.cpp
    src/RaZtracer/Render/*.cpp
    src/RaZtracer/Utils/*.cpp

    include/RaZtracer/RaZtracer.hpp
    include/RaZtracer/Math/*.hpp
    include/RaZtracer/Math/*.inl
    include/RaZtracer/Render/*.hpp
    include/RaZtracer/Utils/*.hpp
)

# Defining preprocessor macros and selecting files to be removed
if (UNIX)
    add_definitions(
        -D_GLFW_X11
        -D_GLFW_HAS_XF86VM

        #-fsanitize=undefined
        #-fsanitize=address
        #-fsanitize=leak
    )

    file(
        GLOB
        DISPOSABLE

        extern/glfw/src/cocoa*
        extern/glfw/src/win32*
        extern/glfw/src/wl*
        extern/glfw/src/wgl*
        extern/glfw/src/mir*
        extern/glfw/src/*.m
    )

    set(
        LIBS

        #asan
        #ubsan
    )

    set(
        DEPS

        X11
        GL
        Xrandr
        Xcursor
        Xinerama
        Xxf86vm
        pthread
        dl
    )
elseif (WIN32)
    add_definitions(
        -D_GLFW_WIN32
        -DGLEW_BUILD
        #-DGLEW_NO_GLU
    )

    file(
        GLOB
        DISPOSABLE

        extern/glfw/src/cocoa*
        extern/glfw/src/x11*
        extern/glfw/src/wl*
        extern/glfw/src/mir*
        extern/glfw/src/glx*
        extern/glfw/src/linux*
        extern/glfw/src/posix*
        extern/glfw/src/*.m
    )

    set(
        DEPS

        opengl32
    )
elseif (APPLE)
    add_definitions(
        -D_GLFW_COCOA
        -D_GLFW_USE_CHDIR
        -D_GLFW_USE_MENUBAR
        -D_GLFW_USE_RETINA
    )

    file(
        GLOB
        DISPOSABLE

        extern/glfw/src/win32*
        extern/glfw/src/x11*
        extern/glfw/src/wl*
        extern/glfw/src/mir*
        extern/glfw/src/glx*
        extern/glfw/src/linux*
    )
endif ()

# Adding recursively every file we want to compile
file(
    GLOB
    SOURCE_FILES

    ${SRC}
)

# Removing unwanted platform-specific files
list(
    REMOVE_ITEM
    SOURCE_FILES

    ${DISPOSABLE}
)

add_executable(RaZtracer ${SOURCE_FILES} examples/main.cpp)

target_link_libraries(
    RaZtracer ${LIBS}
    ${DEPS}
)
