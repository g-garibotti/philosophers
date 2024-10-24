/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 14:33:08 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/24 17:50:24 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

static bool	check_all_ate_enough(t_program *prog)
{
	int		i;
	bool	all_done;

	if (prog->must_eat_count == -1)
		return (false);
	i = 0;
	all_done = true;
	while (i < prog->philo_count)
	{
		if (prog->philos[i].meals_eaten < prog->must_eat_count)
		{
			all_done = false;
			break ;
		}
		i++;
	}
	return (all_done);
}

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
	while (!prog->someone_died && !check_all_ate_enough(prog))
	{
		i = 0;
		while (i < prog->philo_count)
		{
			check_and_mark_death(prog, i);
			if (prog->someone_died)
				break ;
			i++;
		}
		usleep(500);
	}
	return (NULL);
}
