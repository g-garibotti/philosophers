/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 22:24:16 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/18 00:27:53 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

# define SUCCESS 0
# define ERROR 1

typedef struct s_philosopher
{
	int				id;
	int				left_fork;
	int				right_fork;
	int				meals_eaten;
	long long		last_meal_time;
	struct s_data	*data;
	pthread_t		thread;
}					t_philosopher;

typedef struct s_data
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meals_to_eat;
	int				someone_died;
	long long		start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	write_mutex;
	t_philosopher	*philosophers;
}					t_data;

// Init functions
int					init_data(t_data *data, int argc, char **argv);
int					init_mutexes(t_data *data);
int					init_philosophers(t_data *data);

// Action functions
void				philo_eat(t_philosopher *philo);
void				philo_sleep(t_philosopher *philo);
void				philo_think(t_philosopher *philo);
void				*philosopher(void *arg);

// Utility functions
long long			get_time(void);
void				smart_sleep(long long time, t_data *data);
void				print_status(t_data *data, int id, char *status);
int					check_meals(t_data *data);

// Thread functions
int					create_threads(t_data *data);
int					join_threads(t_data *data);

// Monitoring function
void				check_death(t_data *data);

// Cleanup and error handling
void				cleanup_data(t_data *data);
int					error_exit(char *message, t_data *data);

// Additional utility functions
int					ft_atoi(const char *str);

#endif