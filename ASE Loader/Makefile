CC			= g++

TARGET		= ASELoader

$(TARGET):	Main.o Init.o Ase.o
	$(CC) -o $(TARGET) Main.o Init.o Ase.o `sdl-config --libs` -lGL -lGLU -lm

Main.o:		Main.cpp main.h
	$(CC) -c Main.cpp `sdl-config --cflags`

Ase.o:	    Ase. p Ase.h
	$(CC) -c Ase.cpp `sdl-config --cflags`

Init.o:		Init.cpp main.h
	$(CC) -c Init.cpp `sdl-config --cflags`

clean:
	rm -f $(TARGET) Main.o Init.o Ase.o core
