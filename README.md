# pthreads

Winnie the Pooh and bees. A given number of bees extract honey in equal portions, lingering on the way for a random time.
Winnie the Pooh consumes honey in portions of a given size for a given time and can live the same amount of time without food.
The work of each bee is implemented in the generated process.

Compile: 

    make
    or
    gcc game.c -o game -lpthread -w

Run: 

    ./game
    or 
    ./game <count_of_bees> <portion_of_honey> <barrel_of_honey> <delay_winnie_the_pooh> <portion_of_honey_for_winnie>

End of game: 

    CTRL + C
    IMPORTANT: the game will not stop immediately, you need to wait a couple of seconds until all threads are completed

If you don't use parameters, it's will default parameters. `./game 10 1 10 5 5`. All parameters are integer numbers and you can choose any value > 0. 

* barrel_of_honey - the volume of a barrel of honey in which honey bees put honey. If barrel of honey is full, then bees put honey in /dev/null and the bees are sad

* delay_winnie_the_pooh - time for which winnie eats honey in seconds
 
* portion_of_honey_for_winnie - portion of honey, that winnie eats eat for one time
 
* portion_of_honey - portion of honey, that bee put for one time

Different input parameters:

`./game 10 1 10 5 5` - Bees Satisfy Winnie's Honey Needs? he is happy

`./game 10 1 10 3 5` - Bees NOT Satisfy Winnie's Honey Needs, he did not survive