/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 22:24:16 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/22 14:18:17 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_program	t_program;
typedef struct s_philo		t_philo;

typedef struct s_philo
{
	int						id;
	pthread_mutex_t			*left_fork;
	pthread_mutex_t			*right_fork;
	long long				last_meal_time;
	int						meals_eaten;
	t_program				*prog;
}							t_philo;

typedef struct s_program
{
	t_philo					*philos;
	pthread_mutex_t			*forks;
	pthread_mutex_t			death_mutex;
	pthread_mutex_t			print_mutex;
	bool					someone_died;
	int						philo_count;
	long long				start_time;
	int						time_to_die;
	int						time_to_eat;
	int						time_to_sleep;
	int						must_eat_count;
}							t_program;

// Core functions
bool						init_program(t_program *prog, int argc,
								char **argv);
bool						start_simulation(t_program *prog);

// Utils
long long					get_time(void);
void						smart_sleep(long long ms);
void						print_status(t_philo *philo, char *status);
bool						is_simulation_over(t_program *prog);
int							ft_atoi(const char *str);

// Philosopher routines
bool						philosopher_eat(t_philo *philo);
void						philosopher_sleep(t_philo *philo);
void						philosopher_think(t_philo *philo);

#endif
