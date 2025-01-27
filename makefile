all: game
	gcc -g -o projet -Wall main.c ./game/game.c ./game/entity_types/entity_types.c ./game/entity_types/entity_type_vector.c ./game/round.c ./game/display.c

clean:
	rm projet