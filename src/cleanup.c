/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 00:12:21 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/18 00:35:54 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

static void	destroy_mutexes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data->write_mutex);
}

void	cleanup_data(t_data *data)
{
	if (data->forks)
	{
		destroy_mutexes(data);
		free(data->forks);
	}
	if (data->philosophers)
		free(data->philosophers);
}

void	error_exit(char *message, t_data *data)
{
	if (message)
		ft_putendl_fd(message, 2);
	if (data)
		cleanup_data(data);
	exit(EXIT_FAILURE);
}