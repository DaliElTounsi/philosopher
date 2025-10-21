/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_monitor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohchams <mohchams@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 18:09:56 by mohchams          #+#    #+#             */
/*   Updated: 2025/10/13 00:00:00 by mohchams         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_death(t_philo *philo)
{
	int	dead;

	pthread_mutex_lock(philo->table->death_mutex);
	dead = philo->table->someone_died;
	pthread_mutex_unlock(philo->table->death_mutex);
	return (dead);
}

int	check_meals_done(t_philo *philo)
{
	int	meals;

	if (philo->table->nb_meals_required == -1)
		return (0);
	pthread_mutex_lock(philo->meal_mutex);
	meals = philo->meals_eaten;
	pthread_mutex_unlock(philo->meal_mutex);
	if (meals >= philo->table->nb_meals_required)
		return (1);
	return (0);
}

int	check_philo_death(t_philo *philo)
{
	struct timeval	current_time;
	long			time_since_eat;

	gettimeofday(&current_time, NULL);
	pthread_mutex_lock(philo->meal_mutex);
	time_since_eat = get_time_diff(philo->time_last_eat, current_time);
	pthread_mutex_unlock(philo->meal_mutex);
	if (time_since_eat > philo->table->time_to_die)
		return (1);
	return (0);
}

void	*handle_single_philo(t_philo *philo)
{
	print_log(philo, "has taken a fork");
	while (!check_death(philo))
		break_usleep(philo, 1);
	return (NULL);
}

void	*death_monitor(void *arg)
{
	t_table	*table;

	table = (t_table *)arg;
	while (1)
	{
		if (!check_all_philos_alive(table))
			return (NULL);
		if (all_philos_ate_enough(table))
		{
			set_death_flag(table);
			return (NULL);
		}
		usleep(100);
	}
	return (NULL);
}
