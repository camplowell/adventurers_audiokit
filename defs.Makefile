# Copyright (c) 2021 Lowell Camp
# 
# This file is part of the Adventurer's Audiokit,
# which is licensed under the Mozilla Public License 2.0
# See https://opensource.org/licenses/MPL-2.0

# This Makefile provides some global definitions and properties
# shared across all project buildfiles.

##---------------------------------------------------------------------
## GLOBAL PROPERTIES
##---------------------------------------------------------------------


CC = clang
CXX = clang++

BINDIR := bin
DOCDIR := doc
INCLUDES :=

FLAGS = 
CFLAGS = $(FLAGS) -std=c17 -MD -MP
CPPFLAGS = $(FLAGS) -std=c++17

##---------------------------------------------------------------------
## FORMATTING
##---------------------------------------------------------------------
# Text color codes
#Black        0;30     Dark Gray     1;30
#Red          0;31     Light Red     1;31
#Green        0;32     Light Green   1;32
#Brown/Orange 0;33     Yellow        1;33
#Blue         0;34     Light Blue    1;34
#Purple       0;35     Light Purple  1;35
#Cyan         0;36     Light Cyan    1;36
#Light Gray   0;37     White         1;37
p_no  = \033[0m
p_red = \033[0;31m
p_green = \033[0;32m
p_blue = \033[0;34m
p_purple = \033[0;35m
p_gray = \033[1;30m
p_bold = \033[1m

##---------------------------------------------------------------------
## COMPILATION
##---------------------------------------------------------------------

OBJS = $(patsubst %, $(BINDIR)/%.o, $(basename $(SRCS)))
INCTAGS = $(addprefix -I, $(INCLUDES))

COMPILE.cpp = $(CXX) $(INCTAGS) $(CPPFLAGS) -c -MD -MP -MF $(@:.o=.d) -o $@  $<
COMPILE.c   = $(CC)  $(INCTAGS) $(CFLAGS)   -c -MD -MP -MF $(@:.o=.d) -o $@ $<

COMP_MESSAGE = @printf "$(p_purple)Compiling$(p_no) $<\n $(p_blue)Include tags:$(p_no) $(INCTAGS)\n"