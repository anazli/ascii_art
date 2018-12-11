# Makefile for simulation's 2 executable (main)
EXEDIR=.
all : $(EXEDIR)/main 
# The compiler
CC=g++

#Variables
DEBUG=-g
OPTIM=-O3
ISO=gnu++14
PAR=-fopenmp

IDIR=include
ODIR=obj
#SDIR=src
VPATH=src include

# Compiler flags:
CFLAGS=-I$(IDIR) 
FLAGS= $(DEBUG) -std=$(ISO)
LFLAGS=-I$(ODIR)

#Linking object files
_DEPS = stb_image.h stb_image_resize.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ1 = main.o stb_image.o stb_image_resize.o
OBJ1 = $(patsubst %,$(ODIR)/%,$(_OBJ1))

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) $(FLAGS) -c -o $@ $< $(CFLAGS) 

$(EXEDIR)/main: $(OBJ1)
	$(CC) $(FLAGS) -o $@ $^ $(LFLAGS) 

.PHONY: clean

clean :
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
