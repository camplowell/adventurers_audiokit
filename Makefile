-include defs.Makefile

##---------------------------------------------------------------------
## CONFIGURATION
##---------------------------------------------------------------------

CC := clang
CXX := clang++

EXE = adventurers_audiokit

SRC := src/main.cpp

INCLUDE_DIRS = src lib/imgui lib/imgui/backends lib/libsoundio lib/libsoundio/build

 # Ensure the specified directories exist
# Mirrors the file structure in the objs and deps
$(shell mkdir -p $(BIN_DIR))
$(shell mkdir -p $(DEPS_DIR))

$(shell mkdir -p $(dir $(OBJS)))
$(shell mkdir -p $(dir $(DEPS)))

##---------------------------------------------------------------------
## PREPROCESSING
##---------------------------------------------------------------------

OBJS := $(patsubst %, $(BIN_DIR)/%.o, $(basename $(SRCS)))

DEPS := $(patsubst $, $(BIN_DIR)/%.d, $(basename $(SRCS)))

CXXFLAGS = $(addprefix -I, $(INCLUDE_DIRS))

CXXFLAGS += -g -Wall -Wformat

# Flag to find dependencies
DEPFLAGS = -MT $@ -MD -MP -MF $(DEPS_DIR)/$<.Td
# Compile a c file
COMPILE.c = $(CC) $(DEPFLAGS) $(CFLAGS) -c -o $@
# Compile a c++ file
COMPILE.cc = $(CXX) $(DEPFLAGS) $(CXXFLAGS) -c -o $@
# Convert dependencies transactionally
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d
# Link o files
LINK.o = $(LD) $(LDFLAGS) $(LDLIBS) -o $@

CFLAGS = 
LIBS =

##---------------------------------------------------------------------
## BUILD FLAGS PER PLATFORM
##---------------------------------------------------------------------

ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += -lGL `pkg-config --static --libs glfw3`

	CXXFLAGS += `pkg-config --cflags glfw3`
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	ECHO_MESSAGE = "Mac OS X"
	LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
	LIBS += -L/usr/local/lib -L/opt/local/lib -L/opt/homebrew/lib
	#LIBS += -lglfw3
	LIBS += -lglfw

	CXXFLAGS += -I/usr/local/include -I/opt/local/include -I/opt/homebrew/include
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(OS), Windows_NT) #WINDOWS
	ECHO_MESSAGE = "MinGW"
	LIBS += -lglfw3 -lgdi32 -lopengl32 -limm32

	CXXFLAGS += `pkg-config --cflags glfw3`
	CFLAGS = $(CXXFLAGS)
endif

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

all: 
	@echo Making all
	$(MAKE) -f lib/imgui.Makefile all