#include "philo.h"

void *philosopher_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;

    while (1) // Boucle infinie, à ajuster avec condition de fin
    {
        // Penser
        print_log(philo, "is thinking");

        // Prendre fourchettes
        if (philo->ID % 2 == 0)
        {
            pthread_mutex_lock(philo->left_fork);
            print_log(philo, "has taken a fork");
            pthread_mutex_lock(philo->right_fork);
            print_log(philo, "has taken a fork");
        }
        else
        {
            pthread_mutex_lock(philo->left_fork);
            print_log(philo, "has taken a fork");
            pthread_mutex_lock(philo->right_fork);
            print_log(philo, "has taken a fork");
        }
        // Manger
        gettimeofday(&philo->time_last_eat, NULL); // Mise à jour du dernier repas
        print_log(philo, "is eating");
        usleep(philo->table->time_to_eat * 1000); // Temps en microsecondes

        // Relâcher fourchettes
        pthread_mutex_unlock(philo->left_fork);
        pthread_mutex_unlock(philo->right_fork);

        // Dormir
        print_log(philo, "is sleeping");
        usleep(philo->table->time_to_sleep * 1000);

        // Vérifier la mort (simplifié)
        struct timeval current_time;
        gettimeofday(&current_time, NULL);
        long elapsed = (current_time.tv_sec - philo->time_last_eat.tv_sec) * 1000 +
                       (current_time.tv_usec - philo->time_last_eat.tv_usec) / 1000;
        if (elapsed > philo->table->time_to_die)
        {
            print_log(philo, "died");
            break; // Arrêt de la routine, à améliorer pour arrêter tous les threads
        }
    }
    return (NULL);
}