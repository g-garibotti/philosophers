/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_actions.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 12:12:17 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/21 12:56:27 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

static void	print_status(t_simulation_data *sim_data, int id, char *status)
{
	pthread_mutex_lock(&sim_data->print_mutex);
	if (!sim_data->sim_stop)
		printf("%lld %d %s\n", get_current_time() - sim_data->start_time, id,
			status);
	pthread_mutex_unlock(&sim_data->print_mutex);
}

bool	check_death(t_simulation_data *sim_data, t_philosopher *philo)
{
	pthread_mutex_lock(&sim_data->death_mutex);
	if (sim_data->sim_stop || time_diff(philo->last_meal_time,
			get_current_time()) > sim_data->time_to_die)
	{
		if (!sim_data->sim_stop)
		{
			sim_data->sim_stop = true;
			print_status(sim_data, philo->id, "died");
		}
		pthread_mutex_unlock(&sim_data->death_mutex);
		return (true);
	}
	pthread_mutex_unlock(&sim_data->death_mutex);
	return (false);
}

bool	philosopher_eat(t_simulation_data *sim_data, t_philosopher *philo)
{
	pthread_mutex_lock(&sim_data->forks[philo->left_fork]);
	print_status(sim_data, philo->id, "has taken left fork");
	pthread_mutex_lock(&sim_data->forks[philo->right_fork]);
	print_status(sim_data, philo->id, "has taken right fork");
	pthread_mutex_lock(&sim_data->death_mutex);
	if (sim_data->sim_stop)
	{
		pthread_mutex_unlock(&sim_data->death_mutex);
		pthread_mutex_unlock(&sim_data->forks[philo->right_fork]);
		pthread_mutex_unlock(&sim_data->forks[philo->left_fork]);
		return (false);
	}
	print_status(sim_data, philo->id, "is eating");
	philo->last_meal_time = get_current_time();
	philo->eat_count++;
	pthread_mutex_unlock(&sim_data->death_mutex);
	precise_sleep(sim_data->time_to_eat);
	pthread_mutex_unlock(&sim_data->forks[philo->right_fork]);
	pthread_mutex_unlock(&sim_data->forks[philo->left_fork]);
	return (!check_death(sim_data, philo));
}

void	philosopher_sleep(t_simulation_data *sim_data, t_philosopher *philo)
{
	print_status(sim_data, philo->id, "is sleeping");
	precise_sleep(sim_data->time_to_sleep);
}

void	philosopher_think(t_simulation_data *sim_data, t_philosopher *philo)
{
	print_status(sim_data, philo->id, "is thinking");
}
