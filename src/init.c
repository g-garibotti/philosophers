/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 00:06:34 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/18 00:14:41 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	init_fork_mutexes(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	if (!data->forks)
		return (ERROR);
	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			return (ERROR);
		i++;
	}
	return (SUCCESS);
}

int	init_mutexes(t_data *data)
{
	if (init_fork_mutexes(data) != SUCCESS)
		return (ERROR);
	if (pthread_mutex_init(&data->write_mutex, NULL) != 0)
		return (ERROR);
	return (SUCCESS);
}

int	init_philosophers(t_data *data)
{
	int	i;

	data->philosophers = malloc(sizeof(t_philosopher) * data->num_philos);
	if (!data->philosophers)
		return (ERROR);
	i = 0;
	while (i < data->num_philos)
	{
		data->philosophers[i].id = i + 1;
		data->philosophers[i].left_fork = i;
		data->philosophers[i].right_fork = (i + 1) % data->num_philos;
		data->philosophers[i].meals_eaten = 0;
		data->philosophers[i].last_meal_time = 0;
		data->philosophers[i].data = data;
		i++;
	}
	return (SUCCESS);
}

static int	parse_args(t_data *data, int argc, char **argv)
{
	if (argc < 5 || argc > 6)
		return (ERROR);
	data->num_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->meals_to_eat = ft_atoi(argv[5]);
	else
		data->meals_to_eat = -1;
	return (SUCCESS);
}

int	init_data(t_data *data, int argc, char **argv)
{
	if (parse_args(data, argc, argv) != SUCCESS)
		return (ERROR);
	data->someone_died = 0;
	data->start_time = 0;
	if (init_mutexes(data) != SUCCESS)
		return (ERROR);
	if (init_philosophers(data) != SUCCESS)
		return (ERROR);
	return (SUCCESS);
}
