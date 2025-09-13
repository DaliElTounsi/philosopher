#include "philo.h"

void init_table(t_table *table)
{
    int i;

    i = 0;
    table->forks = malloc(sizeof(pthread_mutex_t) * table->nb_philo);
    table->threads = malloc(sizeof(pthread_t) * table->nb_philo);
    if (!table->forks || !table->threads)
    {
        free(table->forks);
        free(table->threads);
        return;
    }
    while (i < table->nb_philo)
    {
        pthread_mutex_init(&table->forks[i], NULL);
        i++;
    }
}

void init_threads(t_table *table)
{
    int i;

    i = 0;
    while (i < table->nb_philo)
    {
        pthread_create(&table->threads[i], NULL, philosopher_routine, NULL);
        i++;
    }
}

void print_log(t_philo *philo, char *state)
{
    struct timeval current_time;
    long timestamp_ms;

    pthread_mutex_lock(philo->table->log_mutex);
    gettimeofday(&current_time, NULL);
    timestamp_ms = (current_time.tv_sec - philo->table->start_time.tv_sec) * 1000 +
                   (current_time.tv_usec - philo->table->start_time.tv_usec) / 1000;
    printf("%ld %d %s\n", timestamp_ms, philo->ID, state);
    pthread_mutex_unlock(philo->table->log_mutex);
}

int parse_args(t_table *table, int ac, char **av)
{
    int i;
    int *values[4];

    values[0] = &table->nb_philo;
    values[1] = &table->time_to_die;
    values[2] = &table->time_to_eat;
    values[3] = &table->time_to_sleep;

    i = 0;
    while (i < 4)
    {
        *values[i] = atoi(av[i + 1]);
        if (*values[i] <= 0)
        {
            free(table);
            return (1);
        }
        i++;
    }
    if (ac == 6)
    {
        table->nb_meals_required = atoi(av[5]);
        if (table->nb_meals_required <= 0)
        {
            free(table);
            return (1);
        }
    }
    else
    {
        table->nb_meals_required = -1;
    }
    return (0);
}

void wait_for_threads(t_table *table)
{
    int i;

    i = 0;
    while (i < table->nb_philo)
    {
        pthread_detach(table->threads[i]);
        i++;
    }
}

void destroy_mutexes(t_table *table)
{
    int i;

    i = 0;
    while (i < table->nb_philo)
    {
        pthread_mutex_destroy(&table->forks[i]);
        i++;
    }
}

int main(int ac, char **av)
{
    t_table *table;
    int i;

    if (ac != 5 && ac != 6)
        return (1);
    table = malloc(sizeof(t_table));
    if (!table)
        return (1);
    if (parse_args(table, ac, av) != 0)
        return (1);
    table->philos = malloc(sizeof(t_philo) * table->nb_philo);
    if (!table->philos)
    {
        free(table);
        return (1);
    }
    i = 0;
    while (i < table->nb_philo)
    {
        table->philos[i].ID = i + 1;
        table->philos[i].table = table;
        table->philos[i].left_fork = &table->forks[i];
        table->philos[i].right_fork = &table->forks[(i + 1) % table->nb_philo];
        i++;
    }
    gettimeofday(&table->start_time, NULL);
    table->log_mutex = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(table->log_mutex, NULL);
    init_table(table);
    init_threads(table);
    wait_for_threads(table);
    destroy_mutexes(table);
    free(table->forks);
    free(table->threads);
    pthread_mutex_destroy(table->log_mutex);
    free(table->log_mutex);
    free(table->philos);
    free(table);
    return (0);
}