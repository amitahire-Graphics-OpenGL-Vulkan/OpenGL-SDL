CC			= g++ -ggdb

TARGET		= HeightMap4

$(TARGET):	Main.o Init.o Camera.o Terrain.o
	$(CC) -o $(TARGET) Main.o Init.o Camera.o Terrain.o `sdl-config --libs` -lGL -lGLU -lm

Main.o:		Main.cpp main.h
	$(CC) -c Main.cpp `sdl- nfig --cflags`

Terrain.o:	Terrain.cpp Terrain.h
	$(CC) -c Terrain.cpp `sdl-config --cflags`

Init.o:		Init.cpp main.h
	$(CC) -c Init.cpp `sdl-config --cflags`

Camera.o:	Camera.cpp Camera.h
	$(CC) -c Camera.cpp `sdl-config --cflags`
    
clean:
	rm -f $(TARGET) Main.o Init.o Camera.o Terrain.o core
