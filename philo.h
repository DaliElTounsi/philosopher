#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <errno.h>
# include <fcntl.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_philo t_philo; // Déclaration forward de t_philo

typedef struct s_table
{
    int             nb_philo;
    int             time_to_die;
    int             time_to_eat;
    int             time_to_sleep;
    int             nb_meals_required;
    pthread_mutex_t *forks;
    pthread_t       *threads;
    struct timeval  start_time;
    pthread_mutex_t *log_mutex;
    t_philo         *philos; // Pointeur vers le tableau de philosophes
}   t_table;

typedef struct s_philo
{
    int             ID;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    struct timeval  time_last_eat;
    t_table         *table;
}   t_philo;

void    *philosopher_routine(void *arg);
void    print_log(t_philo *philo, char *state);

#endif