/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 10:56:44 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/21 13:10:02 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"
#include <stdlib.h>

static bool	init_philosophers(t_simulation_data *sim_data)
{
	int	i;

	sim_data->philosophers = malloc(sizeof(t_philosopher)
			* sim_data->num_of_philos);
	if (!sim_data->philosophers)
		return (false);
	i = 0;
	while (i < sim_data->num_of_philos)
	{
		sim_data->philosophers[i].id = i + 1;
		sim_data->philosophers[i].left_fork = i;
		sim_data->philosophers[i].right_fork = (i + 1)
			% sim_data->num_of_philos;
		sim_data->philosophers[i].eat_count = 0;
		sim_data->philosophers[i].last_meal_time = 0;
		sim_data->philosophers[i].sim_data = sim_data;
		i++;
	}
	return (true);
}

static bool	init_forks(t_simulation_data *sim_data)
{
	int	i;

	sim_data->forks = malloc(sizeof(pthread_mutex_t) * sim_data->num_of_philos);
	if (!sim_data->forks)
		return (false);
	i = 0;
	while (i < sim_data->num_of_philos)
	{
		if (pthread_mutex_init(&sim_data->forks[i], NULL) != 0)
			return (false);
		i++;
	}
	return (true);
}

static bool	init_mutexes(t_simulation_data *sim_data)
{
	if (pthread_mutex_init(&sim_data->death_mutex, NULL) != 0)
		return (false);
	if (pthread_mutex_init(&sim_data->print_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&sim_data->death_mutex);
		return (false);
	}
	return (true);
}

bool	init_simulation(t_simulation_data *sim_data)
{
	if (!init_philosophers(sim_data))
		cleanup_and_exit(sim_data, "Failed to initialize philosophers");
	if (!init_forks(sim_data))
		cleanup_and_exit(sim_data, "Failed to initialize forks");
	if (!init_mutexes(sim_data))
		cleanup_and_exit(sim_data, "Failed to initialize mutexes");
	return (true);
}
