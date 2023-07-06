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
    g_src/util/basics.cpp \
    g_src/util/command_line.cpp \
    g_src/hooks/dfhooks.cpp \
    g_src/render/enabler.cpp \
    g_src/render/enabler_input.cpp \
    g_src/files/files.cpp \
    g_src/files/find_files_posix.cpp \
    g_src/render/graphics.cpp \
    g_src/audio/music_and_sound.cpp \
    g_src/render/init.cpp \
    g_src/render/interface.cpp \
    g_src/render/keybindings.cpp \
    g_src/render/KeybindingScreen.cpp \
    g_src/util/random.cpp \
    g_src/render/renderer_offscreen.cpp \
    g_src/render/resize++.cpp \
    g_src/text/textlines.cpp \
    g_src/texture/textures.cpp \
    g_src/render/ViewBase.cpp \
    g_src/platform/win32_compat.cpp \
    glue/glue.cpp \
	$(LINKOS) -o main
	@echo "Done";
