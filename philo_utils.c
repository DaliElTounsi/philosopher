/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohchams <mohchams@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 00:00:00 by mohchams          #+#    #+#             */
/*   Updated: 2025/10/15 17:33:41 by mohchams         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

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

void	break_usleep(t_philo *philo, int time_ms)
{
	struct timeval	start;
	struct timeval	current;
	long			elapsed;

	gettimeofday(&start, NULL);
	while (1)
	{
		pthread_mutex_lock(philo->table->death_mutex);
		if (philo->table->someone_died)
		{
			pthread_mutex_unlock(philo->table->death_mutex);
			break ;
		}
		pthread_mutex_unlock(philo->table->death_mutex);
		gettimeofday(&current, NULL);
		elapsed = get_time_diff(start, current);
		if (elapsed >= time_ms)
			break ;
		usleep(500);
	}
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
