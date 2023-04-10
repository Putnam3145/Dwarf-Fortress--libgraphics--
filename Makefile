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
	$(LINKOS) -o main
	@echo "Done";
