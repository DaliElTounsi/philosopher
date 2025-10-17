/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohchams <mohchams@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 18:10:17 by mohchams          #+#    #+#             */
/*   Updated: 2025/10/13 00:00:00 by mohchams         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_forks(t_philo *philo)
{
	int	first_id;
	int	second_id;

	if (philo->left_fork_id < philo->right_fork_id)
	{
		first_id = philo->left_fork_id;
		second_id = philo->right_fork_id;
	}
	else
	{
		first_id = philo->right_fork_id;
		second_id = philo->left_fork_id;
	}
	pthread_mutex_lock(&philo->table->forks[first_id]);
	print_log(philo, "has taken a fork");
	pthread_mutex_lock(&philo->table->forks[second_id]);
	print_log(philo, "has taken a fork");
}

void	philo_eat(t_philo *philo)
{
	int	first_id;
	int	second_id;

	take_forks(philo);
	pthread_mutex_lock(philo->meal_mutex);
	gettimeofday(&philo->time_last_eat, NULL);
	philo->meals_eaten++;
	pthread_mutex_unlock(philo->meal_mutex);
	print_log(philo, "is eating");
	break_usleep(philo, philo->table->time_to_eat);
	if (philo->left_fork_id < philo->right_fork_id)
	{
		first_id = philo->left_fork_id;
		second_id = philo->right_fork_id;
	}
	else
	{
		first_id = philo->right_fork_id;
		second_id = philo->left_fork_id;
	}
	pthread_mutex_unlock(&philo->table->forks[second_id]);
	pthread_mutex_unlock(&philo->table->forks[first_id]);
}

void	philo_sleep(t_philo *philo)
{
	print_log(philo, "is sleeping");
	break_usleep(philo, philo->table->time_to_sleep);
}

void	philo_think(t_philo *philo)
{
	long	think_time;

	print_log(philo, "is thinking");
	think_time = philo->table->time_to_eat - philo->table->time_to_sleep;
	if (think_time > 0)
		break_usleep(philo, think_time);
	else
		break_usleep(philo, 1);
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->table->nb_philo == 1)
		return (handle_single_philo(philo));
	if (philo->id % 2 == 0)
	{
		print_log(philo, "is thinking");
		break_usleep(philo, philo->table->time_to_eat / 2);
	}
	while (!check_death(philo))
	{
		if (check_meals_done(philo))
			break ;
		philo_eat(philo);
		if (check_meals_done(philo))
			break ;
		philo_sleep(philo);
		philo_think(philo);
	}
	return (NULL);
}
