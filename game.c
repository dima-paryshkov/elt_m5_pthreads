#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <malloc.h>

/*
 * barrel_of_honey - the volume of a barrel of honey in which
 *                   honey bees put honey. If barrel of honey
 *                   is full, then bees put honey in /dev/null
 *                   and the bees are sad
 *
 * delay_winnie_the_pooh - time for which winnie eats honey in
 *                         seconds
 *
 * portion_of_honey_for_winnie - portion of honey, that winnie
 *                               eats eat for one time
 *
 * portion_of_honey - portion of honey, that bee put for one time
 *
 * mutex - limiter for a barrel of honey
 *
 * ./game <count_of_bees> <portion_of_honey> <max_barrel_of_honey> <delay_winnie_the_pooh> <portion_of_honey_for_winnie>
 *
 */

int barrel_of_honey;
int max_barrel_of_honey;
int delay_winnie_the_pooh;
int portion_of_honey;
int portion_of_honey_for_winnie;
int count_of_bees;
int *bees_num;
int flag;

pthread_mutex_t mutex;

int init(int argc, char **argv)
{
    flag = 1;
    barrel_of_honey = 0;

    /* default option*/
    if (argc == 1)
    {
        max_barrel_of_honey = 10;
        delay_winnie_the_pooh = 5;
        portion_of_honey_for_winnie = 5;
        portion_of_honey = 1;
        count_of_bees = 10;
    }
    else if (argc == 6)
    {
        count_of_bees = atoi(argv[2]);
        portion_of_honey = atoi(argv[3]);
        max_barrel_of_honey = atoi(argv[4]);
        delay_winnie_the_pooh = atoi(argv[5]);
        portion_of_honey_for_winnie = atoi(argv[6]);
    }
    else
    {
        fprintf(stderr, "Error(./%s): incorrect count of arguments\n", argv[0]);
        fprintf(stderr, "Format of arguments: ./%s <count_of_bees> <portion_of_honey> <barrel_of_honey> <delay_winnie_the_pooh> <portion_of_honey_for_winnie>\n", argv[0]);
        return EXIT_FAILURE;
    }

    bees_num = (int*)malloc(count_of_bees * sizeof(int));
    if (bees_num == NULL)
    {
        fprintf(stderr, "Error(./%s): malloc() return NULL\n", argv[0]);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < count_of_bees; i++)
        bees_num[i] == i;

    pthread_mutex_init(&mutex, NULL);
    return EXIT_SUCCESS;
}

void *bees_routine(void *arg)
{
    int bee_num = *(int*)arg;
    int delay_for_bee;

    while (flag)
    {
        delay_for_bee = rand() % 5;
        sleep(delay_for_bee);
        pthread_mutex_lock(&mutex);
        if (portion_of_honey == max_barrel_of_honey)
        {
            printf("bee[%d]: +1 to /dev/null. Volume of honey is max(%d)\n", bee_num, max_barrel_of_honey);
        }
        else   
        {
            barrel_of_honey += portion_of_honey;
            printf("bee[%d]: +1 to honey. Volume of honey: %d\n", bee_num, portion_of_honey);
        }
        pthread_mutex_unlock(&mutex);
    }
}

int main(int argc, char **argv)
{
    if (init(argc, argv) == EXIT_FAILURE)
    {
        fprintf(stderr, "Error(%s): init()\n", argv[0]);
        return EXIT_FAILURE;
    }

    free(bees_num);
}