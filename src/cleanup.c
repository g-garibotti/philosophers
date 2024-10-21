/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 10:53:42 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/21 13:19:47 by ggaribot         ###   ########.fr       */
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

	printf("Debug: Entering cleanup_resources\n");
	if (sim_data->philosophers)
	{
		i = 0;
		while (i < sim_data->num_of_philos)
		{
			if (!sim_data->philosophers[i].thread_joined)
			{
				printf("Debug: Attempting to join thread for philosopher %d\n", i + 1);
				if (pthread_join(sim_data->philosophers[i].thread, NULL) != 0)
				{
					printf("Debug: Failed to join thread for philosopher %d\n", i + 1);
				}
				else
				{
					printf("Debug: Successfully joined thread for philosopher %d\n", i + 1);
					sim_data->philosophers[i].thread_joined = 1;
				}
			}
			else
			{
				printf("Debug: Thread for philosopher %d already joined\n", i + 1);
			}
			i++;
		}
	}
	printf("Debug: Finished joining philosopher threads\n");

	if (sim_data->forks)
	{
		i = 0;
		while (i < sim_data->num_of_philos)
		{
			printf("Debug: Destroying mutex for fork %d\n", i + 1);
			pthread_mutex_destroy(&sim_data->forks[i]);
			i++;
		}
	}
	printf("Debug: Destroying death and print mutexes\n");
	pthread_mutex_destroy(&sim_data->death_mutex);
	pthread_mutex_destroy(&sim_data->print_mutex);
	printf("Debug: Freeing simulation data\n");
	free_simulation_data(sim_data);
	printf("Debug: Cleanup complete\n");
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
