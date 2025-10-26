/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohchams <mohchams@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 00:00:00 by mohchams          #+#    #+#             */
/*   Updated: 2025/10/26 00:00:00 by mohchams         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	cleanup_philos_partial(t_table *table, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (table->philos[i].meal_mutex)
		{
			pthread_mutex_destroy(table->philos[i].meal_mutex);
			free(table->philos[i].meal_mutex);
		}
		i++;
	}
	free(table->philos);
}

void	cleanup_forks_full(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nb_philo)
	{
		pthread_mutex_destroy(&table->forks[i]);
		i++;
	}
	free(table->forks);
}

static void	cleanup_basic(t_table *table)
{
	pthread_mutex_destroy(table->death_mutex);
	pthread_mutex_destroy(table->log_mutex);
	free(table->death_mutex);
	free(table->log_mutex);
	free(table->threads);
}

static int	init_basic(t_table *table)
{
	table->threads = malloc(sizeof(pthread_t) * table->nb_philo);
	if (!table->threads)
		return (0);
	table->log_mutex = malloc(sizeof(pthread_mutex_t));
	if (!table->log_mutex)
	{
		free(table->threads);
		return (0);
	}
	table->death_mutex = malloc(sizeof(pthread_mutex_t));
	if (!table->death_mutex)
	{
		free(table->log_mutex);
		free(table->threads);
		return (0);
	}
	pthread_mutex_init(table->log_mutex, NULL);
	pthread_mutex_init(table->death_mutex, NULL);
	return (1);
}

int	init_table(t_table *table)
{
	if (!init_basic(table))
		return (0);
	table->someone_died = 0;
	gettimeofday(&table->start_time, NULL);
	if (!init_forks(table))
	{
		cleanup_basic(table);
		return (0);
	}
	if (!init_philos(table))
	{
		cleanup_forks_full(table);
		cleanup_basic(table);
		return (0);
	}
	return (1);
}
