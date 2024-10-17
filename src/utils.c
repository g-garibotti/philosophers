/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 00:19:19 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/18 00:24:10 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	smart_sleep(long long time, t_data *data)
{
	long long	i;

	i = get_time();
	while (!data->someone_died)
	{
		if (get_time() - i >= time)
			break ;
		usleep(50);
	}
}

void	print_status(t_data *data, int id, char *status)
{
	pthread_mutex_lock(&data->write_mutex);
	if (!data->someone_died)
		printf("%lld %d %s\n", get_time() - data->start_time, id, status);
	pthread_mutex_unlock(&data->write_mutex);
}

int	check_meals(t_data *data)
{
	int	i;
	int	finished_eating;

	i = 0;
	finished_eating = 0;
	while (i < data->num_philos)
	{
		if (data->philosophers[i].meals_eaten >= data->meals_to_eat)
			finished_eating++;
		i++;
	}
	if (finished_eating == data->num_philos)
		return (SUCCESS);
	return (ERROR);
}
