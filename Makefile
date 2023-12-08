SRC_DIR = src
OBJ_DIR = ./obj
EXE=checkers.exe

SRC=$(wildcard *.cpp $(foreach fd, $(SRC_DIR), $(fd)/*.cpp)) 
OBJS=$(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))


CPP = g++
DESTDIR ?=
PREFIX ?= /usr
OPT_LEVEL ?= -O2 
CFLAGS ?= -g -DWIN32 -DSHOW_FPS -Wall -Wextra `sdl-config --cflags`
LDFLAGS ?= -g -L$(PREFIX)/lib
LDLIBS ?=  `sdl-config --libs` -lSDL_image -lSDL_ttf -lSDL_mixer -lSDL -lSDL_gfx -lm -mconsole

#MINGW does not have X11 and does not require it
#dont know about cygwin
ifneq ($(OS),Windows_NT)
LDLIBS += -lX11
endif


.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJS)
	$(CPP) $(LDFLAGS) $(TARGET_ARCH) $(OBJS) -o $@ $(LDLIBS)

$(OBJ_DIR)/%.o: %.cpp
	mkdir -p $(@D)
	$(CPP) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR):
	mkdir -p $@

clean:
	$(RM) -rv *~ $(OBJS) $(EXE)