# Copyright (c) 2021 Lowell Camp
# 
# This file is part of the Adventurer's Audiokit,
# which is licensed under the Mozilla Public License 2.0
# See https://opensource.org/licenses/MPL-2.0

# This Makefile is responsible for managing the compilation of Libsoundio
# and placing the built files in the bin directory.

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
	$(COMPILE.c)