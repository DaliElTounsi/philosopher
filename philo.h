/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohchams <mohchams@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 18:11:02 by mohchams          #+#    #+#             */
/*   Updated: 2025/10/17 22:53:32 by mohchams         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <errno.h>
# include <fcntl.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_philo	t_philo;

typedef struct s_table
{
	int					nb_philo;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					nb_meals_required;
	pthread_mutex_t		*forks;
	pthread_t			*threads;
	pthread_t			monitor_thread;
	struct timeval		start_time;
	pthread_mutex_t		*log_mutex;
	pthread_mutex_t		*death_mutex;
	int					someone_died;
	t_philo				*philos;
}	t_table;

typedef struct s_philo
{
	int					id;
	int					left_fork_id;
	int					right_fork_id;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	pthread_mutex_t		*meal_mutex;
	struct timeval		time_last_eat;
	int					meals_eaten;
	t_table				*table;
}	t_philo;

int		is_valid_number(const char *str);
int		ft_atoi(const char *str);
int		parse_args(t_table *table, int ac, char **av);
int		init_forks(t_table *table);
int		init_philos(t_table *table);
int		init_table(t_table *table);
void	error_exit(t_table *table, char *msg);
long	get_time_diff(struct timeval start, struct timeval end);
void	print_log(t_philo *philo, char *state);
void	break_usleep(t_philo *philo, int time_ms);
int		start_simulation(t_table *table);
void	cleanup(t_table *table);
void	destroy_resources(t_table *table);
void	take_forks(t_philo *philo);
void	philo_eat(t_philo *philo);
void	philo_sleep(t_philo *philo);
void	philo_think(t_philo *philo);
void	*philosopher_routine(void *arg);
int		check_death(t_philo *philo);
int		check_meals_done(t_philo *philo);
int		check_philo_death(t_philo *philo);
void	*handle_single_philo(t_philo *philo);
void	*death_monitor(void *arg);
void	set_death_flag(t_table *table);
int		all_philos_ate_enough(t_table *table);
int		check_all_philos_alive(t_table *table);
void	print_death(t_philo *philo);

#endif