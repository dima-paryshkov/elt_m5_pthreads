#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <malloc.h>
#include <unistd.h>
#include <signal.h>
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
        delay_winnie_the_pooh = 4;
        portion_of_honey_for_winnie = 5;
        portion_of_honey = 1;
        count_of_bees = 10;
    }
    else if (argc == 6)
    {
        for (int i = 1; i < argc; i++)
            if (atoi(argv[i]) < 0)
            {
                fprintf(stderr, "Error(%s): incorrect value of arguments. All the arguments should be > 0\n", argv[0]);
                return EXIT_FAILURE;
            }

        count_of_bees = atoi(argv[1]);
        portion_of_honey = atoi(argv[2]);
        max_barrel_of_honey = atoi(argv[3]);
        delay_winnie_the_pooh = atoi(argv[4]);
        portion_of_honey_for_winnie = atoi(argv[5]);

        if (portion_of_honey_for_winnie > max_barrel_of_honey)
        {
            fprintf(stderr, "Error(%s): incorrect value of arguments. Winnie can't eats more, than max volume barrel of honey", argv[0]);
            return EXIT_FAILURE;
        }
    }
    else
    {
        fprintf(stderr, "Error(%s): incorrect count of arguments\n", argv[0]);
        fprintf(stderr, "Format of arguments: %s <count_of_bees> <portion_of_honey> <barrel_of_honey> <delay_winnie_the_pooh> <portion_of_honey_for_winnie>\n", argv[0]);
        return EXIT_FAILURE;
    }

    bees_num = (int *)malloc(count_of_bees * sizeof(int));
    if (bees_num == NULL)
    {
        fprintf(stderr, "Error(%s): malloc() return NULL\n", argv[0]);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < count_of_bees; i++)
        bees_num[i] = i;

    pthread_mutex_init(&mutex, NULL);
    return EXIT_SUCCESS;
}

void *bees_routine(void *arg)
{
    int bee_num = (int)arg;
    int delay_for_bee;

    while (flag)
    {
        delay_for_bee = 2 + rand() % 5;
        sleep(delay_for_bee);
        pthread_mutex_lock(&mutex);
        if (barrel_of_honey == max_barrel_of_honey)
        {
            printf("bee[%d]: +1 to /dev/null. Volume of honey is max(%d)\n", bee_num, max_barrel_of_honey);
        }
        else
        {
            barrel_of_honey += portion_of_honey;
            printf("bee[%d]: +1 to honey. Volume of honey: %d\n", bee_num, barrel_of_honey);
        }
        pthread_mutex_unlock(&mutex);
    }
}

void *winnie_routine()
{
    /* winnie is live :) */
    while (flag)
    {
        sleep(delay_winnie_the_pooh);
        pthread_mutex_lock(&mutex);
        if (portion_of_honey_for_winnie > barrel_of_honey)
        {
            printf("winnie: Winnie did not have enough honey, he did not survive (((\n");
            printf("\n-----GAME OVER-----\n");
            flag = 0;
            sleep(1);
            pthread_exit(NULL);
        }
        else
        {
            barrel_of_honey -= portion_of_honey_for_winnie;
            printf("winnie: Winnie is happy because he ate honey. Volume of honey: %d\n", barrel_of_honey);
        }
        pthread_mutex_unlock(&mutex);
    }
}

void signalHandler(int signum)
{
    flag = 0;
    printf("\n----- GAME FIHISHED-----\n");
}

int main(int argc, char **argv)
{
    signal(SIGINT, signalHandler);
    printf("\n----- GAME STARTED-----\n");
    if (init(argc, argv) == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }
    pthread_t thread[count_of_bees];
    for (int i = 0; i < count_of_bees; i++)
    {
        if (pthread_create(thread + i, NULL, bees_routine, (void *)(intptr_t)(bees_num[i])) == -1)
        {
            perror("pthread_create:");
            return EXIT_FAILURE;
        }
    }
    pthread_t thread_winnie;
    if (pthread_create(&thread_winnie, NULL, winnie_routine, NULL) == -1)
    {
        perror("pthread_create:");
        return EXIT_FAILURE;
    }
    pthread_join(thread_winnie, NULL);

    free(bees_num);
    return EXIT_SUCCESS;
}