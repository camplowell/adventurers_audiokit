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

INCLUDES = $(BASE_DIR) $(BASE_DIR)/build

##---------------------------------------------------------------------
## AUTOMATED TASKS
##---------------------------------------------------------------------

# Add base directory to sources
SRCS := $(addprefix $(BASE_DIR)/, $(SRCS))

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

all:
	@mkdir -p $(BASE_DIR)/build
	cd $(BASE_DIR)/build; cmake ..
