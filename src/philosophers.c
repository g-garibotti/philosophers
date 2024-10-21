/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 10:50:16 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/21 14:53:35 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

t_simulation_data	*initialize_simulation(int argc, char **argv)
{
	t_simulation_data	*sim_data;

	sim_data = (t_simulation_data *)malloc(sizeof(t_simulation_data));
	if (!sim_data)
		cleanup_and_exit(NULL, "Failed to allocate memory");
	set_simulation_params(sim_data, argc, argv);
	if (!init_simulation(sim_data))
		cleanup_and_exit(sim_data, "Failed to initialize simulation");
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
