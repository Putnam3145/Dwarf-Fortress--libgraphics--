This is not a real cmakelists file! It's just the part of CMakelists that compiles g_src.

cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 20)
set(CURSES_NEED_NCURSES True)

set(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)

find_package(PkgConfig REQUIRED)

add_library(g_src_lib SHARED 
    src/g_src/basics.cpp
    src/g_src/command_line.cpp
    src/g_src/dfhooks.cpp
    src/g_src/enabler.cpp
    src/g_src/enabler_input.cpp
    src/g_src/files.cpp
    src/g_src/find_files_posix.cpp
    src/g_src/graphics.cpp
    src/g_src/music_and_sound.cpp
    src/g_src/init.cpp
    src/g_src/interface.cpp
    src/g_src/keybindings.cpp
    src/g_src/KeybindingScreen.cpp
    src/g_src/random.cpp
    src/g_src/renderer_offscreen.cpp
    src/g_src/resize++.cpp
    src/g_src/textlines.cpp
    src/g_src/textures.cpp
    src/g_src/ViewBase.cpp
    src/g_src/win32_compat.cpp
)

target_link_options(g_src_lib PRIVATE "LINKER:-as-needed")

target_include_directories(g_src_lib PRIVATE ${PROJECT_SOURCE_DIR}/sdl/include ${PROJECT_SOURCE_DIR}/fmod/inc ${PROJECT_SOURCE_DIR}/zlib)

find_package(SDL2 REQUIRED)

target_include_directories(g_src_lib PUBLIC ${SDL2_INCLUDE_DIRS})

target_link_libraries(g_src_lib PUBLIC SDL2::SDL2)

pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

target_include_directories(g_src_lib PUBLIC ${SDL2IMAGE_INCLUDE_DIRS})

target_link_libraries(g_src_lib PUBLIC ${SDL2IMAGE_LIBRARIES})

add_library(fmod_plugin MODULE src/g_src/music_and_sound_fmod.cpp)

find_library(FMOD_LIBRARY REQUIRED NAMES libfmod${CMAKE_SHARED_LIBRARY_SUFFIX} PATHS fmod/lib/x86_64)
target_link_libraries(fmod_plugin PRIVATE ${FMOD_LIBRARY})

add_library(sdl_mixer_plugin MODULE src/g_src/music_and_sound_sdl.cpp)

find_library(SDL_MIXER_LIBRARY REQUIRED
  NAMES SDL2_mixer
  HINTS
    ENV SDLMIXERDIR
    ENV SDLDIR
  PATH_SUFFIXES lib
)

target_include_directories(sdl_mixer_plugin PRIVATE ${SDL2_INCLUDE_DIRS})

target_link_libraries(sdl_mixer_plugin PRIVATE ${SDL_MIXER_LIBRARY})

add_library(zlib STATIC
    zlib/adler32.c
    zlib/compress.c
    zlib/contrib/minizip/ioapi.c
    zlib/contrib/minizip/unzip.c
    zlib/crc32.c
    zlib/deflate.c
    zlib/gzclose.c
    zlib/gzlib.c
    zlib/gzread.c
    zlib/gzwrite.c
    zlib/infback.c
    zlib/inffast.c
    zlib/inflate.c
    zlib/inftrees.c
    zlib/trees.c
    zlib/uncompr.c
    zlib/zutil.c
)