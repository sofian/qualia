
CFLAGS=-Wall -Werror -O0 -g $(shell pkg-config --cflags libmapper-0)
LDLIBS=-lglut -lGLU -lGLEW $(shell pkg-config --libs libmapper-0)

all: vectorfield agentConnect

vectorfield: vectorfield.o vectorfield_opengl.o vectorfield_opengl.h

vectorfield.o: vectorfield.c vectorfield_opengl.h
vectorfield_opengl.o: vectorfield_opengl.c vectorfield_opengl.h
