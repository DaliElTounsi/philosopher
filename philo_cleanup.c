/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_cleanup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohchams <mohchams@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 00:00:00 by mohchams          #+#    #+#             */
/*   Updated: 2025/10/17 00:00:00 by mohchams         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
