/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 00:42:05 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/18 00:49:14 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	create_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_create(&data->philosophers[i].thread, NULL, philosopher,
				&data->philosophers[i]) != 0)
			return (ERROR);
		i++;
	}
	return (SUCCESS);
}

int	join_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_join(data->philosophers[i].thread, NULL) != 0)
			return (ERROR);
		i++;
	}
	return (SUCCESS);
}

void	check_death(t_data *data)
{
	int			i;
	long long	current_time;

	while (!data->someone_died && !check_meals(data))
	{
		i = 0;
		while (i < data->num_philos)
		{
			current_time = get_time();
			if (current_time
				- data->philosophers[i].last_meal_time > data->time_to_die)
			{
				print_status(data, data->philosophers[i].id, "died");
				data->someone_died = 1;
				return ;
			}
			i++;
		}
		usleep(1000);
	}
}
