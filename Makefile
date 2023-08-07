#OBJS specifies which files to compile as part of the project
OBJS = main_wav.o

#CC specifies used compiler
CC = gcc

#INCLUDE_PATHS specifies additional include paths we'll need
INCLUDE_PATHS = -I`sdl2-config --cflags`

#LIBRARY_PATHS specifies additional libraries
LIBRARY_PATHS = -L

#COMPILER_FLAGS specifies additional compilitaion options
COMPILER_FLAGS = 

#LINKER_FLAGS specifies the libraries we're linking to
LINKER_FLAGS = -lSDL2main `sdl2-config --static-libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer

#OBJ_NAME = specifies the name of the executable
OBJ_NAME = main

all: $(OBJS)
	$(CC) $(OBJS) -g $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS) -o $(OBJ_NAME)

main_wav.o: main_wav.c
	$(CC) $(INCLUDE_PATHS) -c main_wav.c

clean:
	rm *.o
