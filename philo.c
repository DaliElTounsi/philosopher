/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohchams <mohchams@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 18:10:43 by mohchams          #+#    #+#             */
/*   Updated: 2025/10/26 20:54:10 by mohchams         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	parse_args(t_table *table, int ac, char **av)
{
	if (!is_valid_number(av[1]) || !is_valid_number(av[2])
		|| !is_valid_number(av[3]) || !is_valid_number(av[4]))
		return (1);
	table->nb_philo = ft_atoi(av[1]);
	table->time_to_die = ft_atoi(av[2]);
	table->time_to_eat = ft_atoi(av[3]);
	table->time_to_sleep = ft_atoi(av[4]);
	if (table->nb_philo <= 0 || table->time_to_die <= 0
		|| table->time_to_eat <= 0 || table->time_to_sleep <= 0)
		return (1);
	if (ac == 6)
	{
		if (!is_valid_number(av[5]))
			return (1);
		table->nb_meals_required = ft_atoi(av[5]);
		if (table->nb_meals_required <= 0)
			return (1);
	}
	else
		table->nb_meals_required = -1;
	return (0);
}

int	main(int ac, char **av)
{
	t_table	*table;

	if (ac != 5 && ac != 6)
	{
		printf("invalid argument");
		return (1);
	}
	table = malloc(sizeof(t_table));
	if (!table)
		return (error_exit(NULL, "Error: malloc failed\n"));
	if (parse_args(table, ac, av) != 0)
	{
		printf("invalid argument");
		free(table);
		return (1);
	}
	if (!init_table(table))
		return (error_exit(table, "Error: initialization failed\n"));
	if (!start_simulation(table))
		return (error_exit(table, "Error: thread creation failed\n"));
	cleanup(table);
	return (0);
}
