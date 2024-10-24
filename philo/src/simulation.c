/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 12:28:15 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/23 17:03:33 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

bool	is_simulation_over(t_program *prog)
{
	bool	should_stop;
	int		i;
	int		finished_count;

	pthread_mutex_lock(&prog->death_mutex);
	should_stop = prog->someone_died;
	if (prog->must_eat_count != -1)
	{
		i = 0;
		finished_count = 0;
		while (i < prog->philo_count)
		{
			if (prog->philos[i].meals_eaten >= prog->must_eat_count)
				finished_count++;
			i++;
		}
		if (finished_count == prog->philo_count)
			should_stop = true;
	}
	pthread_mutex_unlock(&prog->death_mutex);
	return (should_stop);
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->prog->philo_count == 1)
	{
		print_status(philo, "has taken a fork");
		while (!is_simulation_over(philo->prog))
			usleep(100);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		usleep(1000);
	while (!is_simulation_over(philo->prog))
	{
		check_and_mark_death(philo->prog, philo->id - 1);
		if (is_simulation_over(philo->prog))
			break ;
		philosopher_eat(philo);
		philosopher_sleep(philo);
		philosopher_think(philo);
	}
	return (NULL);
}

bool	start_simulation(t_program *prog)
{
	pthread_t	*threads;
	pthread_t	monitor;
	int			i;

	threads = malloc(sizeof(pthread_t) * prog->philo_count);
	if (!threads)
		return (false);
	prog->start_time = get_time();
	i = 0;
	while (i < prog->philo_count)
	{
		if (pthread_create(&threads[i], NULL, philosopher_routine,
				&prog->philos[i]) != 0)
			return (false);
		i++;
	}
	if (pthread_create(&monitor, NULL, death_monitor, prog) != 0)
		return (false);
	pthread_join(monitor, NULL);
	i = 0;
	while (i < prog->philo_count)
		pthread_join(threads[i++], NULL);
	free(threads);
	return (true);
}
