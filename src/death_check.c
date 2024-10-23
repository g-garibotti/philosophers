/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 14:33:08 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/23 16:39:08 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

static bool	has_philosopher_finished_eating(t_program *prog, int i)
{
	if (prog->must_eat_count == -1)
		return (false);
	return (prog->philos[i].meals_eaten >= prog->must_eat_count);
}

void	check_and_mark_death(t_program *prog, int i)
{
	long long	current_time;
	long long	time_since_meal;
	bool		should_print;

	current_time = get_time();
	should_print = false;
	// Always take death_mutex first
	pthread_mutex_lock(&prog->death_mutex);
	if (!has_philosopher_finished_eating(prog, i))
	{
		time_since_meal = current_time - prog->philos[i].last_meal_time;
		if (time_since_meal >= prog->time_to_die && !prog->someone_died)
		{
			prog->someone_died = true;
			should_print = true;
		}
	}
	pthread_mutex_unlock(&prog->death_mutex);
	// Print after releasing death_mutex if needed
	if (should_print)
	{
		pthread_mutex_lock(&prog->print_mutex);
		printf("%lld %d died\n", current_time - prog->start_time,
			prog->philos[i].id);
		pthread_mutex_unlock(&prog->print_mutex);
	}
}

void	*death_monitor(void *arg)
{
	t_program	*prog;
	int			i;
	int			finished_count;

	prog = (t_program *)arg;
	while (1)
	{
		i = 0;
		finished_count = 0;
		while (i < prog->philo_count)
		{
			if (has_philosopher_finished_eating(prog, i))
				finished_count++;
			else
				check_and_mark_death(prog, i);
			pthread_mutex_lock(&prog->death_mutex);
			if (prog->someone_died || (prog->must_eat_count != -1
					&& finished_count == prog->philo_count))
			{
				pthread_mutex_unlock(&prog->death_mutex);
				return (NULL);
			}
			pthread_mutex_unlock(&prog->death_mutex);
			i++;
		}
		usleep(100);
	}
	return (NULL);
}
