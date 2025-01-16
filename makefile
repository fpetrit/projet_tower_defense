all: game
	gcc -g -o projet -Wall main.c ./game/game.c

clean:
	rm projet