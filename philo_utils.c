/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohchams <mohchams@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 00:00:00 by mohchams          #+#    #+#             */
/*   Updated: 2025/10/13 00:00:00 by mohchams         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time_diff(struct timeval start, struct timeval end)
{
	long	diff;

	diff = (end.tv_sec - start.tv_sec) * 1000
		+ (end.tv_usec - start.tv_usec) / 1000;
	return (diff);
}

void	print_log(t_philo *philo, char *state)
{
	struct timeval	current_time;
	long			timestamp_ms;

	pthread_mutex_lock(philo->table->death_mutex);
	if (philo->table->someone_died)
	{
		pthread_mutex_unlock(philo->table->death_mutex);
		return ;
	}
	pthread_mutex_unlock(philo->table->death_mutex);
	pthread_mutex_lock(philo->table->log_mutex);
	gettimeofday(&current_time, NULL);
	timestamp_ms = (current_time.tv_sec
			- philo->table->start_time.tv_sec) * 1000
		+ (current_time.tv_usec - philo->table->start_time.tv_usec) / 1000;
	printf("%ld %d %s\n", timestamp_ms, philo->id, state);
	pthread_mutex_unlock(philo->table->log_mutex);
}

void	start_simulation(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nb_philo)
	{
		pthread_create(&table->threads[i], NULL,
			philosopher_routine, &table->philos[i]);
		i++;
	}
	pthread_create(&table->monitor_thread, NULL, death_monitor, table);
}

void	cleanup(t_table *table)
{
	int	i;

	pthread_join(table->monitor_thread, NULL);
	i = 0;
	while (i < table->nb_philo)
	{
		pthread_join(table->threads[i], NULL);
		i++;
	}
	destroy_resources(table);
}

void	destroy_resources(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nb_philo)
	{
		pthread_mutex_destroy(&table->forks[i]);
		pthread_mutex_destroy(table->philos[i].meal_mutex);
		free(table->philos[i].meal_mutex);
		i++;
	}
	pthread_mutex_destroy(table->log_mutex);
	pthread_mutex_destroy(table->death_mutex);
	free(table->log_mutex);
	free(table->death_mutex);
	free(table->forks);
	free(table->threads);
	free(table->philos);
	free(table);
}
