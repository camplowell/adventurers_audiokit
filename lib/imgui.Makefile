# Copyright (c) 2021 Lowell Camp
# 
# This file is part of the Adventurer's Audiokit,
# which is licensed under the Mozilla Public License 2.0
# See https://opensource.org/licenses/MPL-2.0

# This Makefile is responsible for managing the compilation of Dear IMGUI
# and placing the built files in the bin directory.

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