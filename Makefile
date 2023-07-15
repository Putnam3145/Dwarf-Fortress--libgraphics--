# Dwarf-Fortress Libgraphics Makefile
# Tested on MacOSX (M1, arm) GNU/Linux (Ubuntu, x86 amd)

# Install Location Configuration
INCPATH = -I/usr/local/include
LIBPATH = -L/usr/local/lib
LINKOS = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lz -ldl -lpthread

# Compilation Settings
CC = g++-11 -std=c++20
CF = -Wfatal-errors -O3 -g -DDF_GLUE_CPP

UNAME := $(shell uname)
ifeq ($(UNAME), Linux)			#Detect GNU/Linux
endif
ifeq ($(UNAME), Darwin)			#Detect MacOS
# assume dependency installation w. homebrew
INCPATH = -I/opt/homebrew/include
LIBPATH = -L/opt/homebrew/lib
CC = g++-12 -std=c++20
endif

.PHONY: all
all:
	@echo "Compiling Dwarf-Fortress libgraphics (main)...";
	@$(CC) $(CF) $(INCPATH) $(LIBPATH) \
    g_src/platform/win32_compat.cpp \
    g_src/util/definitions.cpp \
    g_src/util/command_line.cpp \
    g_src/util/random.cpp \
    g_src/util/logger.cpp \
    g_src/text/stringvec.cpp \
    g_src/text/textlines.cpp \
    g_src/text/transform.cpp \
    g_src/files/files.cpp \
    g_src/files/file_compressor.cpp \
    g_src/files/find_files_posix.cpp \
    g_src/files/find_files_windows.cpp \
    g_src/audio/audio.cpp \
    g_src/audio/audio_fmod.cpp \
    g_src/audio/audio_sdl.cpp \
    g_src/graphics/enabler.cpp \
    g_src/graphics/enabler_input.cpp \
    g_src/graphics/graphics.cpp \
    g_src/graphics/init.cpp \
    g_src/graphics/interface.cpp \
    g_src/graphics/keybindings.cpp \
    g_src/graphics/KeybindingScreen.cpp \
    g_src/graphics/renderer_offscreen.cpp \
    g_src/graphics/resize++.cpp \
    g_src/graphics/textures.cpp \
    g_src/graphics/ViewBase.cpp \
    g_src/hooks/dfhooks.cpp \
    glue/glue.cpp \
	$(LINKOS) -o main
	@echo "Done";
