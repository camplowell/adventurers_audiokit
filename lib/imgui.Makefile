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

##---------------------------------------------------------------------
## BASIC CONFIGURATION
##---------------------------------------------------------------------

BASE_DIR := lib/imgui

SRCS := imgui.cpp imgui_draw.cpp imgui_tables.cpp imgui_widgets.cpp

INCLUDES := $(BASE_DIR)

##---------------------------------------------------------------------
## AUTOMATED TASKS
##---------------------------------------------------------------------

# Add base directory to sources
SRCS := $(addprefix $(BASE_DIR)/, $(SRCS))

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

all: $(OBJS)

$(BINDIR)/%.o: %.cpp
#   Create any required directories and announce compilation
	@mkdir -p $(dir $@)
	$(COMP_MESSAGE)
#   Compile the corresponding c++ file
	$(COMPILE.cpp) -o $@ $<