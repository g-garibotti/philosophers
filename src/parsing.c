/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:51:14 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/21 16:42:07 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

static bool	is_positive_int(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
		i++;
	}
	return (i > 0 && ft_atoi(str) > 0);
}

bool	validate_arguments(int argc, char **argv)
{
	int	i;

	if (argc != 5 && argc != 6)
		return (false);
	i = 1;
	while (i < argc)
	{
		if (!is_positive_int(argv[i]))
			return (false);
		i++;
	}
	return (true);
}
void	set_simulation_params(t_simulation_data *sim_data, int argc,
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
	sim_data->finished_eating_count = 0;
}
