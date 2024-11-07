/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 14:33:08 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/07 10:56:30 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	check_and_mark_death(t_program *prog, int i)
{
	long long	current_time;
	long long	time_since_meal;

	current_time = get_time();
	pthread_mutex_lock(&prog->death_mutex);
	time_since_meal = current_time - prog->philos[i].last_meal_time;
	if (time_since_meal >= prog->time_to_die && !prog->someone_died)
	{
		prog->someone_died = true;
		pthread_mutex_lock(&prog->print_mutex);
		printf("%lld %d died\n", current_time - prog->start_time,
			prog->philos[i].id);
		pthread_mutex_unlock(&prog->print_mutex);
	}
	pthread_mutex_unlock(&prog->death_mutex);
}

void	*death_monitor(void *arg)
{
	t_program	*prog;
	int			i;

	prog = (t_program *)arg;
	while (!is_simulation_over(prog))
	{
		i = 0;
		while (i < prog->philo_count)
		{
			check_and_mark_death(prog, i);
			pthread_mutex_lock(&prog->death_mutex);
			if (prog->someone_died)
			{
				pthread_mutex_unlock(&prog->death_mutex);
				break ;
			}
			pthread_mutex_unlock(&prog->death_mutex);
			i++;
		}
		usleep(500);
	}
	return (NULL);
}
