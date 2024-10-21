/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 22:24:16 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/21 15:21:57 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philosopher		t_philosopher;
typedef struct s_simulation_data	t_simulation_data;

struct								s_philosopher
{
	int								id;
	int								left_fork;
	int								right_fork;
	int								eat_count;
	long long						last_meal_time;
	pthread_t						thread;
	t_simulation_data				*sim_data;
	bool							thread_created;
};

struct								s_simulation_data
{
	int								num_of_philos;
	int								time_to_die;
	int								time_to_eat;
	int								time_to_sleep;
	int								num_times_to_eat;
	bool							sim_stop;
	long long						start_time;
	t_philosopher					*philosophers;
	pthread_mutex_t					*forks;
	pthread_mutex_t					death_mutex;
	pthread_mutex_t					print_mutex;
	pthread_t						death_checker_thread;
	bool							death_checker_created;
};

// Main functions
bool								validate_arguments(int argc, char **argv);
t_simulation_data					*initialize_simulation(int argc,
										char **argv);

// Cleanup functions
void								cleanup_and_exit(t_simulation_data *sim_data,
										char *message);
void								cleanup_resources(t_simulation_data *sim_data);
void								free_simulation_data(t_simulation_data *sim_data);

// Initialization functions
bool								init_simulation(t_simulation_data *sim_data);

// Time utils functions
long long							get_current_time(void);
void								precise_sleep(long long ms);
long long							time_diff(long long past,
										long long present);

// Philosopher actions
bool								philosopher_eat(t_simulation_data *sim_data,
										t_philosopher *philo);
void								philosopher_sleep(t_simulation_data *sim_data,
										t_philosopher *philo);
void								philosopher_think(t_simulation_data *sim_data,
										t_philosopher *philo);
void								print_status(t_simulation_data *sim_data,
										int id, char *status);

// Simulation functions
bool								start_simulation(t_simulation_data *sim_data);
bool								check_death(t_simulation_data *sim_data);

// Utils functions
int									ft_strcmp(const char *s1, const char *s2);
int									ft_atoi(const char *str);

bool								validate_arguments(int argc, char **argv);
void								set_simulation_params(t_simulation_data *sim_data,
										int argc, char **argv);

#endif
