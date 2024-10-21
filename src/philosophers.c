/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 10:50:16 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/21 13:25:42 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"
#include <stdlib.h>

int	ft_atoi(const char *str)
{
	int	result;
	int	sign;

	result = 0;
	sign = 1;
	while ((*str <= 13 && *str >= 9) || *str == ' ')
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	return (sign * result);
}

bool	validate_arguments(int argc, char **argv)
{
	int	i;

	if (argc != 5 && argc != 6)
		return (false);
	i = 1;
	while (i < argc)
	{
		if (ft_atoi(argv[i]) <= 0)
			return (false);
		i++;
	}
	return (true);
}

static void	set_simulation_params(t_simulation_data *sim_data, int argc, char **argv)
{
	printf("Debug: Setting simulation parameters\n");
	sim_data->num_of_philos = ft_atoi(argv[1]);
	sim_data->time_to_die = ft_atoi(argv[2]);
	sim_data->time_to_eat = ft_atoi(argv[3]);
	sim_data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		sim_data->num_times_to_eat = ft_atoi(argv[5]);
	else
		sim_data->num_times_to_eat = -1;
	sim_data->sim_stop = false;
	printf("Debug: Simulation parameters set: %d philosophers, %d ms to die, %d ms to eat, %d ms to sleep\n",
		   sim_data->num_of_philos, sim_data->time_to_die, sim_data->time_to_eat, sim_data->time_to_sleep);
}

t_simulation_data	*initialize_simulation(int argc, char **argv)
{
	t_simulation_data	*sim_data;

	printf("Debug: Entering initialize_simulation\n");
	sim_data = (t_simulation_data *)malloc(sizeof(t_simulation_data));
	if (!sim_data)
	{
		printf("Debug: Failed to allocate memory for sim_data\n");
		cleanup_and_exit(NULL, "Failed to allocate memory");
	}
	printf("Debug: Memory allocated for sim_data\n");
	set_simulation_params(sim_data, argc, argv);
	printf("Debug: Simulation parameters set\n");
	if (!init_simulation(sim_data))
	{
		printf("Debug: Failed to initialize simulation\n");
		cleanup_and_exit(sim_data, "Failed to initialize simulation");
	}
	printf("Debug: Simulation initialized successfully\n");
	return (sim_data);
}

int	main(int argc, char **argv)
{
	t_simulation_data	*sim_data;

	printf("Debug: Starting program\n");
	if (!validate_arguments(argc, argv))
		return (printf("Error: Invalid arguments\n"), 1);
	printf("Debug: Arguments validated\n");
	sim_data = initialize_simulation(argc, argv);
	if (!sim_data)
	{
		printf("Debug: Failed to initialize simulation\n");
		return (1);
	}
	printf("Debug: Simulation initialized\n");
	if (!start_simulation(sim_data))
	{
		printf("Debug: Failed to start simulation\n");
		cleanup_and_exit(sim_data, "Error: Failed to start simulation\n");
		return (1);
	}
	printf("Debug: Simulation completed, starting cleanup\n");
	cleanup_and_exit(sim_data, NULL);
	printf("Debug: Program finished\n");
	return (0);
}
