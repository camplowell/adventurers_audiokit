# Copyright (c) 2021 Lowell Camp
# 
# This file is part of the Adventurer's Audiokit,
# which is licensed under the Mozilla Public License 2.0
# See https://opensource.org/licenses/MPL-2.0

#
# Cross Platform Makefile
# Compatible with MSYS2/MINGW, Ubuntu 14.04.1 and Mac OS X
#
# You will need GLFW (http://www.glfw.org):
# Linux:
#   apt-get install libglfw-dev
# Mac OS X:
#   brew install glfw
# MSYS2:
#   pacman -S --noconfirm --needed mingw-w64-x86_64-toolchain mingw-w64-x86_64-glfw
#

include defs.Makefile

EXE = adventurers_audiokit

BASE_DIR := src

SRCS := main.cpp adventurers_audiokit.cpp \
backends/imgui_impl_glfw.cpp backends/imgui_impl_opengl2.cpp \
audio/audio_main.cpp audio/audio_controls.cpp \
audio/node.cpp

INCLUDES := src lib/imgui

CXXFLAGS = -std=c++17
LIBS = 

DONE_MSG = @printf "\nDone.\n\n"

##---------------------------------------------------------------------
## AUTOMATED MANDATORY PROCESSES
##---------------------------------------------------------------------

# Add base directory to sources
SRCS := $(addprefix $(BASE_DIR)/, $(SRCS))

ALLOBJS = $(shell find $(BINDIR) -name '*.o')

##---------------------------------------------------------------------
## BUILD FLAGS PER PLATFORM
##---------------------------------------------------------------------
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += -lGL `pkg-config --static --libs glfw3`

	CXXFLAGS += `pkg-config --cflags glfw3`
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	ECHO_MESSAGE = "Mac OS X"
	LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
	LIBS += -framework CoreAudio -framework AudioUnit
	LIBS += -L/usr/local/lib -L/opt/local/lib -L/opt/homebrew/lib
	#LIBS += -lglfw3
	LIBS += -lglfw

	CXXFLAGS += -I/usr/local/include -I/opt/local/include -I/opt/homebrew/include
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(OS), Windows_NT)
	ECHO_MESSAGE = "MinGW"
	LIBS += -lglfw3 -lgdi32 -lopengl32 -limm32

	CXXFLAGS += `pkg-config --cflags glfw3`
	CFLAGS = $(CXXFLAGS)
endif

##---------------------------------------------------------------------
## MAKE RULES
##---------------------------------------------------------------------

.PHONY: all
all: $(EXE)
	$(DONE_MSG)

.PHONY: clean
clean:
	rm -rf $(BINDIR)/*
	rm -rf $(DOCDIR)/*
	rm -f $(EXE)
	$(DONE_MSG)

.PHONY: doc
doc: $(BASE_DIR)
	doxygen Doxygen_Config

# Compile all source code (if applicable)
.PHONY: compall
compall:
	@printf "$(p_green)Compiling imgui$(p_no)\n"
	$(MAKE) -f lib/imgui.Makefile all
	@printf "\n\n"
	@printf "$(p_green)Compiling libsoundio$(p_no)\n"
	$(MAKE) -f lib/soundio.Makefile all
	@printf "\n\n"
	@printf "$(p_green)Compiling src$(p_no)\n"
	make $(OBJS)

# Tell the linker to link the exe
$(EXE): compall
	@printf "$(p_blue)Linking$(p_no) $@\n"
	$(CXX) -o $@ $(shell find $(BINDIR) -name '*.o') $(CXXFLAGS) $(LIBS)

$(BINDIR)/%.o: %.cpp
#   Create any required directories and announce compilation
	@mkdir -p $(dir $@)
	$(COMP_MESSAGE)
#   Compile the corresponding c++ file
	$(COMPILE.cpp) -o $@ $<