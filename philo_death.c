/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_death.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohchams <mohchams@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 00:00:00 by mohchams          #+#    #+#             */
/*   Updated: 2025/10/13 00:00:00 by mohchams         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_death_flag(t_table *table)
{
	pthread_mutex_lock(table->death_mutex);
	table->someone_died = 1;
	pthread_mutex_unlock(table->death_mutex);
}

int	all_philos_ate_enough(t_table *table)
{
	int	i;
	int	meals;

	if (table->nb_meals_required == -1)
		return (0);
	i = 0;
	while (i < table->nb_philo)
	{
		pthread_mutex_lock(table->philos[i].meal_mutex);
		meals = table->philos[i].meals_eaten;
		pthread_mutex_unlock(table->philos[i].meal_mutex);
		if (meals < table->nb_meals_required)
			return (0);
		i++;
	}
	return (1);
}

int	check_all_philos_alive(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nb_philo)
	{
		if (check_philo_death(&table->philos[i]))
		{
			print_death(&table->philos[i]);
			set_death_flag(table);
			return (0);
		}
		i++;
	}
	return (1);
}

void	print_death(t_philo *philo)
{
	struct timeval	current_time;
	long			timestamp_ms;

	pthread_mutex_lock(philo->table->log_mutex);
	gettimeofday(&current_time, NULL);
	timestamp_ms = (current_time.tv_sec
			- philo->table->start_time.tv_sec) * 1000
		+ (current_time.tv_usec - philo->table->start_time.tv_usec) / 1000;
	printf("%ld %d died\n", timestamp_ms, philo->id);
	pthread_mutex_unlock(philo->table->log_mutex);
}
