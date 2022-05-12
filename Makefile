default : game.c
	gcc game.c -o game -lpthread -w
	@echo "Format of arguments: ./game <count_of_bees> <portion_of_honey> <barrel_of_honey> <delay_winnie_the_pooh> <portion_of_honey_for_winnie>"
