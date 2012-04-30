
CFLAGS=-Wall -Werror -O0 -g $(shell pkg-config --cflags libmapper-0)
LDLIBS=$(shell pkg-config --libs libmapper-0)
FRAMEWORKS=$(wildcard /System/Library/Frameworks)

ifeq ($(FRAMEWORKS),/System/Library/Frameworks)
CFLAGS += -I/System/Library/Frameworks/OpenGL.framework/Headers \
          -I/System/Library/Frameworks/GLUT.framework/Headers
LDLIBS += -framework OpenGL -framework GLUT
else
LDLIBS += -lglut -lGLU -lGLEW
endif

all: vectorfield agentConnect

vectorfield: vectorfield.o vectorfield_opengl.o vectorfield_opengl.h

vectorfield.o: vectorfield.c vectorfield_opengl.h
vectorfield_opengl.o: vectorfield_opengl.c vectorfield_opengl.h
