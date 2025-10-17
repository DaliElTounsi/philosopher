/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohchams <mohchams@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 18:10:43 by mohchams          #+#    #+#             */
/*   Updated: 2025/10/13 00:00:00 by mohchams         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	parse_args(t_table *table, int ac, char **av)
{
	table->nb_philo = ft_atoi(av[1]);
	table->time_to_die = ft_atoi(av[2]);
	table->time_to_eat = ft_atoi(av[3]);
	table->time_to_sleep = ft_atoi(av[4]);
	if (table->nb_philo <= 0 || table->time_to_die <= 0
		|| table->time_to_eat <= 0 || table->time_to_sleep <= 0)
		return (1);
	if (ac == 6)
	{
		table->nb_meals_required = ft_atoi(av[5]);
		if (table->nb_meals_required <= 0)
			return (1);
	}
	else
		table->nb_meals_required = -1;
	return (0);
}

void	init_forks(t_table *table)
{
	int	i;

	i = 0;
	table->forks = malloc(sizeof(pthread_mutex_t) * table->nb_philo);
	if (!table->forks)
		return ;
	while (i < table->nb_philo)
	{
		pthread_mutex_init(&table->forks[i], NULL);
		i++;
	}
}

void	init_philos(t_table *table)
{
	int	i;

	i = 0;
	table->philos = malloc(sizeof(t_philo) * table->nb_philo);
	if (!table->philos)
		return ;
	while (i < table->nb_philo)
	{
		table->philos[i].id = i + 1;
		table->philos[i].table = table;
		table->philos[i].left_fork_id = i;
		table->philos[i].right_fork_id = (i + 1) % table->nb_philo;
		table->philos[i].left_fork = &table->forks[i];
		table->philos[i].right_fork = &table->forks[(i + 1) % table->nb_philo];
		table->philos[i].meals_eaten = 0;
		table->philos[i].meal_mutex = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(table->philos[i].meal_mutex, NULL);
		gettimeofday(&table->philos[i].time_last_eat, NULL);
		i++;
	}
}

void	init_table(t_table *table)
{
	table->threads = malloc(sizeof(pthread_t) * table->nb_philo);
	table->log_mutex = malloc(sizeof(pthread_mutex_t));
	table->death_mutex = malloc(sizeof(pthread_mutex_t));
	if (!table->threads || !table->log_mutex || !table->death_mutex)
		return ;
	pthread_mutex_init(table->log_mutex, NULL);
	pthread_mutex_init(table->death_mutex, NULL);
	table->someone_died = 0;
	gettimeofday(&table->start_time, NULL);
	init_forks(table);
	init_philos(table);
}

int	main(int ac, char **av)
{
	t_table	*table;

	if (ac != 5 && ac != 6)
		return (1);
	table = malloc(sizeof(t_table));
	if (!table)
		return (1);
	if (parse_args(table, ac, av) != 0)
	{
		free(table);
		return (1);
	}
	init_table(table);
	start_simulation(table);
	cleanup(table);
	return (0);
}
