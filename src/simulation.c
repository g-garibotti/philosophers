/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 12:28:15 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/21 15:53:20 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	*philosopher_routine(void *arg)
{
	t_philosopher		*philo;
	t_simulation_data	*sim_data;

	philo = (t_philosopher *)arg;
	sim_data = philo->sim_data;
	if (sim_data->num_of_philos == 1)
	{
		print_status(sim_data, philo->id, "has taken a fork");
		precise_sleep(sim_data->time_to_die);
		print_status(sim_data, philo->id, "died");
		pthread_mutex_lock(&sim_data->death_mutex);
		sim_data->sim_stop = true;
		pthread_mutex_unlock(&sim_data->death_mutex);
		return (NULL);
	}
	usleep(philo->id * 100);
	while (!check_death(sim_data))
	{
		if (!philosopher_eat(sim_data, philo))
			break ;
		if (check_death(sim_data))
			break ;
		philosopher_sleep(sim_data, philo);
		if (check_death(sim_data))
			break ;
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
	long long			time_since_last_meal;
	long long			current_time;

	sim_data = (t_simulation_data *)arg;
	while (!check_death(sim_data))
	{
		i = -1;
		current_time = get_current_time();
		while (++i < sim_data->num_of_philos)
		{
			pthread_mutex_lock(&sim_data->death_mutex);
			time_since_last_meal = time_diff(sim_data->philosophers[i].last_meal_time,
					current_time);
			if (time_since_last_meal > sim_data->time_to_die + 1)
			// Add 1ms buffer
			{
				sim_data->sim_stop = true;
				print_status(sim_data, sim_data->philosophers[i].id, "died");
				pthread_mutex_unlock(&sim_data->death_mutex);
				return (NULL);
			}
			if (sim_data->num_times_to_eat != -1
				&& sim_data->philosophers[i].eat_count >= sim_data->num_times_to_eat)
			{
				sim_data->finished_eating_count++;
				if (sim_data->finished_eating_count == sim_data->num_of_philos)
				{
					sim_data->sim_stop = true;
					pthread_mutex_unlock(&sim_data->death_mutex);
					return (NULL);
				}
			}
			pthread_mutex_unlock(&sim_data->death_mutex);
		}
		usleep(100);
	}
	return (NULL);
}

bool	start_simulation(t_simulation_data *sim_data)
{
	int	i;

	sim_data->start_time = get_current_time();
	i = -1;
	while (++i < sim_data->num_of_philos)
	{
		sim_data->philosophers[i].last_meal_time = sim_data->start_time;
		if (pthread_create(&sim_data->philosophers[i].thread, NULL,
				philosopher_routine, &sim_data->philosophers[i]) != 0)
		{
			sim_data->sim_stop = true;
			return (false);
		}
	}
	if (sim_data->num_of_philos > 1)
	{
		if (pthread_create(&sim_data->death_checker_thread, NULL, death_checker,
				sim_data) != 0)
		{
			sim_data->sim_stop = true;
			return (false);
		}
		pthread_join(sim_data->death_checker_thread, NULL);
	}
	i = -1;
	while (++i < sim_data->num_of_philos)
	{
		pthread_join(sim_data->philosophers[i].thread, NULL);
	}
	return (true);
}
