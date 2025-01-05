all:
	g++ -Isrc/SDL/include/SDL2 -Isrc/SDL_ttf/include/SDL2 -Lsrc/SDL/lib -Lsrc/SDL_ttf/lib -o main main.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf

run:
	./main

clean:
	del /f main.exe
