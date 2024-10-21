/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 10:56:44 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/21 13:29:01 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"
#include <stdlib.h>

static bool	init_philosophers(t_simulation_data *sim_data)
{
	int	i;

	printf("Debug: Initializing philosophers\n");
	sim_data->philosophers = malloc(sizeof(t_philosopher) * sim_data->num_of_philos);
	if (!sim_data->philosophers)
	{
		printf("Debug: Failed to allocate memory for philosophers\n");
		return (false);
	}
	i = 0;
	while (i < sim_data->num_of_philos)
	{
		sim_data->philosophers[i].id = i + 1;
		sim_data->philosophers[i].left_fork = i;
		sim_data->philosophers[i].right_fork = (i + 1) % sim_data->num_of_philos;
		sim_data->philosophers[i].eat_count = 0;
		sim_data->philosophers[i].last_meal_time = get_current_time();  // Initialize to current time
		sim_data->philosophers[i].sim_data = sim_data;
		sim_data->philosophers[i].thread_joined = 0;
		i++;
	}
	printf("Debug: Philosophers initialized\n");
	return (true);
}

static bool	init_forks(t_simulation_data *sim_data)
{
	int	i;

	printf("Debug: Initializing forks\n");
	sim_data->forks = malloc(sizeof(pthread_mutex_t) * sim_data->num_of_philos);
	if (!sim_data->forks)
	{
		printf("Debug: Failed to allocate memory for forks\n");
		return (false);
	}
	i = 0;
	while (i < sim_data->num_of_philos)
	{
		if (pthread_mutex_init(&sim_data->forks[i], NULL) != 0)
		{
			printf("Debug: Failed to initialize mutex for fork %d\n", i);
			return (false);
		}
		i++;
	}
	printf("Debug: Forks initialized\n");
	return (true);
}

static bool	init_mutexes(t_simulation_data *sim_data)
{
	printf("Debug: Initializing mutexes\n");
	if (pthread_mutex_init(&sim_data->death_mutex, NULL) != 0)
	{
		printf("Debug: Failed to initialize death mutex\n");
		return (false);
	}
	if (pthread_mutex_init(&sim_data->print_mutex, NULL) != 0)
	{
		printf("Debug: Failed to initialize print mutex\n");
		pthread_mutex_destroy(&sim_data->death_mutex);
		return (false);
	}
	printf("Debug: Mutexes initialized\n");
	return (true);
}

bool	init_simulation(t_simulation_data *sim_data)
{
	printf("Debug: Initializing simulation\n");
	if (!init_philosophers(sim_data))
	{
		printf("Debug: Failed to initialize philosophers\n");
		return (false);
	}
	printf("Debug: Philosophers initialized\n");
	if (!init_forks(sim_data))
	{
		printf("Debug: Failed to initialize forks\n");
		return (false);
	}
	printf("Debug: Forks initialized\n");
	if (!init_mutexes(sim_data))
	{
		printf("Debug: Failed to initialize mutexes\n");
		return (false);
	}
	printf("Debug: Mutexes initialized\n");
	return (true);
}
