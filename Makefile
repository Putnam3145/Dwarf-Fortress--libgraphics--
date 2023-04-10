# Dwarf-Fortress Libgraphics Makefile
# Author: @weigert
# Version 1.0
# Tested on MacOSX (amd, M1 arm) (soon: GNU/Linux)

# Install Location Configuration
INCPATH = -I/usr/local/include
LIBPATH = -L/usr/local/lib
LINKOS = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lz

# Compilation Settings
CC = g++-10 -std=c++20
CF = -Wfatal-errors -O3 -g -DDF_GLUE_CPP

# MacOS: Dependencies install with homegrew

UNAME := $(shell uname)
ifeq ($(UNAME), Linux)			#Detect GNU/Linux
endif
ifeq ($(UNAME), Darwin)			#Detext MacOS
INCPATH = -I/opt/homebrew/include
LIBPATH = -L/opt/homebrew/lib
CC = g++-12 -std=c++20
endif

.PHONY: all
all:
	@echo "Dwarf-Fortress libgraphics ...";
	@$(CC) $(CF) $(INCPATH) $(LIBPATH) $(LINKOS) \
    g_src/basics.cpp \
    g_src/command_line.cpp \
    g_src/dfhooks.cpp \
    g_src/enabler.cpp \
    g_src/enabler_input.cpp \
    g_src/files.cpp \
    g_src/find_files_posix.cpp \
    g_src/graphics.cpp \
    g_src/music_and_sound.cpp \
    g_src/init.cpp \
    g_src/interface.cpp \
    g_src/keybindings.cpp \
    g_src/KeybindingScreen.cpp \
    g_src/random.cpp \
    g_src/renderer_offscreen.cpp \
    g_src/resize++.cpp \
    g_src/textlines.cpp \
    g_src/textures.cpp \
    g_src/ViewBase.cpp \
    g_src/win32_compat.cpp \
    glue/glue.cpp \
	-o main
	@echo "Done";