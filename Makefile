include defs.Makefile

EXE = adventurers_audiokit

BASE_DIR := src

SRCS := main.cpp backends/imgui_impl_glfw.cpp backends/imgui_impl_opengl2.cpp

INCLUDES := src lib/imgui

CXXFLAGS = 
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

.PHONY: compall
compall:
	@printf "$(p_green)Compiling imgui$(p_no)\n"
	$(MAKE) -f lib/imgui.Makefile all
	@printf "\n\n"
	@printf "$(p_green)Compiling src$(p_no)\n"
	make $(OBJS)

.PHONY: clean
clean:
	rm -r $(BINDIR)/*
	rm $(EXE)
	$(DONE_MSG)

$(EXE): compall
	@printf "$(p_blue)Linking$(p_no) $@\n"
	$(CXX) -o $@ $(shell find $(BINDIR) -name '*.o') $(CXXFLAGS) $(LIBS)

# Compilation rules defined in defs.Makefile