/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 10:53:42 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/21 12:54:21 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	free_simulation_data(t_simulation_data *sim_data)
{
	if (sim_data)
	{
		if (sim_data->philosophers)
			free(sim_data->philosophers);
		if (sim_data->forks)
			free(sim_data->forks);
		free(sim_data);
	}
}

void	cleanup_resources(t_simulation_data *sim_data)
{
	int	i;

	if (sim_data->philosophers)
	{
		i = 0;
		while (i < sim_data->num_of_philos)
		{
			pthread_join(sim_data->philosophers[i].thread, NULL);
			i++;
		}
	}
	if (sim_data->forks)
	{
		i = 0;
		while (i < sim_data->num_of_philos)
		{
			pthread_mutex_destroy(&sim_data->forks[i]);
			i++;
		}
	}
	pthread_mutex_destroy(&sim_data->death_mutex);
	pthread_mutex_destroy(&sim_data->print_mutex);
	free_simulation_data(sim_data);
}

void	cleanup_and_exit(t_simulation_data *sim_data, char *message)
{
	if (sim_data)
		cleanup_resources(sim_data);
	if (message)
	{
		printf("Error: %s\n", message);
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
