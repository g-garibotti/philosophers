/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 14:33:08 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/24 17:19:43 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

static bool	check_if_all_ate_enough(t_program *prog)
{
	int	i;
	int	finished_count;

	finished_count = 0;
	i = 0;
	while (i < prog->philo_count)
	{
		if (prog->philos[i].meals_eaten >= prog->must_eat_count)
			finished_count++;
		i++;
	}
	return (finished_count == prog->philo_count);
}

void	check_and_mark_death(t_program *prog, int i)
{
	long long	current_time;
	long long	time_since_meal;

	current_time = get_time();
	pthread_mutex_lock(&prog->death_mutex);
	if (prog->must_eat_count == -1
		|| prog->philos[i].meals_eaten < prog->must_eat_count)
	{
		time_since_meal = current_time - prog->philos[i].last_meal_time;
		if (time_since_meal >= prog->time_to_die && !prog->someone_died)
		{
			prog->someone_died = true;
			pthread_mutex_lock(&prog->print_mutex);
			printf("%lld %d died\n", current_time - prog->start_time,
				prog->philos[i].id);
			pthread_mutex_unlock(&prog->print_mutex);
		}
	}
	pthread_mutex_unlock(&prog->death_mutex);
}

static bool	check_initial_conditions(t_program *prog)
{
	bool	should_stop;

	pthread_mutex_lock(&prog->death_mutex);
	should_stop = prog->someone_died;
	if (!should_stop && prog->must_eat_count != -1)
		should_stop = check_if_all_ate_enough(prog);
	pthread_mutex_unlock(&prog->death_mutex);
	return (should_stop);
}

static bool	check_death_occurred(t_program *prog)
{
	pthread_mutex_lock(&prog->death_mutex);
	if (prog->someone_died)
	{
		pthread_mutex_unlock(&prog->death_mutex);
		return (true);
	}
	pthread_mutex_unlock(&prog->death_mutex);
	return (false);
}

void	*death_monitor(void *arg)
{
	t_program	*prog;
	int			i;

	prog = (t_program *)arg;
	while (1)
	{
		i = 0;
		if (check_initial_conditions(prog))
			return (NULL);
		while (i < prog->philo_count)
		{
			check_and_mark_death(prog, i);
			if (check_death_occurred(prog))
				return (NULL);
			i++;
		}
		usleep(100);
	}
}
