/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohchams <mohchams@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 00:00:00 by mohchams          #+#    #+#             */
/*   Updated: 2025/10/20 00:00:00 by mohchams         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_valid_number(const char *str)
{
	int	i;

	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+')
		i++;
	if (str[i] < '0' || str[i] > '9')
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	init_forks(t_table *table)
{
	int	i;

	i = 0;
	table->forks = malloc(sizeof(pthread_mutex_t) * table->nb_philo);
	if (!table->forks)
		return (0);
	while (i < table->nb_philo)
	{
		pthread_mutex_init(&table->forks[i], NULL);
		i++;
	}
	return (1);
}

static void	init_philo_data(t_philo *philo, t_table *table, int i)
{
	philo->id = i + 1;
	philo->table = table;
	philo->left_fork_id = i;
	philo->right_fork_id = (i + 1) % table->nb_philo;
	philo->left_fork = &table->forks[i];
	philo->right_fork = &table->forks[(i + 1) % table->nb_philo];
	philo->meals_eaten = 0;
	gettimeofday(&philo->time_last_eat, NULL);
}

int	init_philos(t_table *table)
{
	int	i;

	i = 0;
	table->philos = malloc(sizeof(t_philo) * table->nb_philo);
	if (!table->philos)
		return (0);
	while (i < table->nb_philo)
	{
		init_philo_data(&table->philos[i], table, i);
		table->philos[i].meal_mutex = malloc(sizeof(pthread_mutex_t));
		if (!table->philos[i].meal_mutex)
		{
			cleanup_philos_partial(table, i);
			return (0);
		}
		pthread_mutex_init(table->philos[i].meal_mutex, NULL);
		i++;
	}
	return (1);
}
