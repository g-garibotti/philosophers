/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 10:53:42 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/21 14:46:06 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	cleanup_resources(t_simulation_data *sim_data)
{
	int	i;

	if (sim_data)
	{
		if (sim_data->death_checker_created)
			pthread_join(sim_data->death_checker_thread, NULL);
		
		if (sim_data->philosophers)
		{
			i = -1;
			while (++i < sim_data->num_of_philos)
			{
				if (sim_data->philosophers[i].thread_created)
					pthread_join(sim_data->philosophers[i].thread, NULL);
			}
			free(sim_data->philosophers);
		}
		
		if (sim_data->forks)
		{
			i = -1;
			while (++i < sim_data->num_of_philos)
				pthread_mutex_destroy(&sim_data->forks[i]);
			free(sim_data->forks);
		}
		
		pthread_mutex_destroy(&sim_data->death_mutex);
		pthread_mutex_destroy(&sim_data->print_mutex);
		
		free(sim_data);
	}
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
