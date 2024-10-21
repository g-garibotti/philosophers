/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 12:28:15 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/21 13:03:21 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	*philosopher_routine(void *arg)
{
	t_philosopher		*philo;
	t_simulation_data	*sim_data;

	philo = (t_philosopher *)arg;
	sim_data = philo->sim_data;
	if (philo->id % 2 == 0)
		precise_sleep(1);
	while (!sim_data->sim_stop)
	{
		if (!philosopher_eat(sim_data, philo))
			break ;
		philosopher_sleep(sim_data, philo);
		philosopher_think(sim_data, philo);
		if (sim_data->num_times_to_eat != -1
			&& philo->eat_count >= sim_data->num_times_to_eat)
			break ;
	}
	return (NULL);
}

void	*death_checker(void *arg)
{
	t_simulation_data	*sim_data;
	int					i;

	sim_data = (t_simulation_data *)arg;
	while (!sim_data->sim_stop)
	{
		i = 0;
		while (i < sim_data->num_of_philos && !sim_data->sim_stop)
		{
			if (check_death(sim_data, &sim_data->philosophers[i]))
				return (NULL);
			i++;
		}
		usleep(100);
	}
	return (NULL);
}

bool	start_simulation(t_simulation_data *sim_data)
{
	int			i;
	pthread_t	death_checker_thread;

	sim_data->start_time = get_current_time();
	i = 0;
	while (i < sim_data->num_of_philos)
	{
		if (pthread_create(&sim_data->philosophers[i].thread, NULL,
				philosopher_routine, &sim_data->philosophers[i]) != 0)
			return (false);
		i++;
	}
	if (pthread_create(&death_checker_thread, NULL, death_checker,
			sim_data) != 0)
		return (false);
	i = 0;
	while (i < sim_data->num_of_philos)
	{
		if (pthread_join(sim_data->philosophers[i].thread, NULL) != 0)
			return (false);
		i++;
	}
	if (pthread_join(death_checker_thread, NULL) != 0)
		return (false);
	return (true);
}
