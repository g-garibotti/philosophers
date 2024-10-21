/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 10:50:16 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/21 12:34:19 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"
#include <stdlib.h>

bool	validate_arguments(int argc, char **argv)
{
	int	i;

	if (argc != 5 && argc != 6)
		return (false);
	i = 1;
	while (i < argc)
	{
		if (atoi(argv[i]) <= 0)
			return (false);
		i++;
	}
	return (true);
}

t_simulation_data	*initialize_simulation(int argc, char **argv)
{
	t_simulation_data	*sim_data;

	sim_data = (t_simulation_data *)malloc(sizeof(t_simulation_data));
	if (!sim_data)
		cleanup_and_exit(sim_data, "Failed to initialize simulation");
	sim_data->num_of_philos = atoi(argv[1]);
	sim_data->time_to_die = atoi(argv[2]);
	sim_data->time_to_eat = atoi(argv[3]);
	sim_data->time_to_sleep = atoi(argv[4]);
	if (argc == 6)
		sim_data->num_times_to_eat = atoi(argv[5]);
	else
		sim_data->num_times_to_eat = -1;
	sim_data->sim_stop = false;
	init_simulation(sim_data);
	return (sim_data);
}

int	main(int argc, char **argv)
{
	t_simulation_data	*sim_data;

	if (!validate_arguments(argc, argv))
		return (printf("Error: Invalid arguments\n"), 1);
	sim_data = initialize_simulation(argc, argv);
	if (!start_simulation(sim_data))
	{
		cleanup_and_exit(sim_data, "Error: Failed to start simulation\n");
		return (1);
	}
	cleanup_and_exit(sim_data, NULL);
	return (0);
}
