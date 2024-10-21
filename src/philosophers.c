/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 10:50:16 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/21 13:04:37 by ggaribot         ###   ########.fr       */
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

static void	set_simulation_params(t_simulation_data *sim_data, int argc,
		char **argv)
{
	sim_data->num_of_philos = ft_atoi(argv[1]);
	sim_data->time_to_die = ft_atoi(argv[2]);
	sim_data->time_to_eat = ft_atoi(argv[3]);
	sim_data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		sim_data->num_times_to_eat = ft_atoi(argv[5]);
	else
		sim_data->num_times_to_eat = -1;
	sim_data->sim_stop = false;
}

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
