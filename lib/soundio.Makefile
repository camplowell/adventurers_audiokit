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

BASE_DIR := lib/libsoundio

SRCS := channel_layout.c coreaudio.c dummy.c os.c ring_buffer.c soundio.c util.c




INCLUDES := $(BASE_DIR) $(BASE_DIR)/build

##---------------------------------------------------------------------
## AUTOMATED TASKS
##---------------------------------------------------------------------

# Add base directory to sources
SRCS := $(addprefix $(BASE_DIR)/src/, $(SRCS))

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

all: checklib complib

.PHONY: checklib
checklib:
	@printf "$(p_purple)Checking for audio libraries$(p_no)\n"
	@mkdir -p $(BASE_DIR)/build
	cd $(BASE_DIR)/build; cmake ..;
.PHONY: complib
complib: $(OBJS)

$(BINDIR)/%.o: %.c
#   Create any required directories and announce compilation
	@mkdir -p $(dir $@)
	$(COMP_MESSAGE)
#   Compile the corresponding c++ file
	$(COMPILE.c) -o $@ $<