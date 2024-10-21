/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_actions.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 12:12:17 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/21 15:53:50 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	print_status(t_simulation_data *sim_data, int id, char *status)
{
	pthread_mutex_lock(&sim_data->print_mutex);
	if (!sim_data->sim_stop || ft_strcmp(status, "died") == 0)
	{
		printf("%lld %d %s\n", get_current_time() - sim_data->start_time, id,
			status);
	}
	pthread_mutex_unlock(&sim_data->print_mutex);
}

bool	check_death(t_simulation_data *sim_data)
{
	bool	is_dead;

	pthread_mutex_lock(&sim_data->death_mutex);
	is_dead = sim_data->sim_stop;
	pthread_mutex_unlock(&sim_data->death_mutex);
	return (is_dead);
}

bool	philosopher_eat(t_simulation_data *sim_data, t_philosopher *philo)
{
	int	first_fork;
	int	second_fork;

	first_fork = (philo->id % 2 == 0) ? philo->right_fork : philo->left_fork;
	second_fork = (philo->id % 2 == 0) ? philo->left_fork : philo->right_fork;
	pthread_mutex_lock(&sim_data->forks[first_fork]);
	print_status(sim_data, philo->id, "has taken a fork");
	if (check_death(sim_data))
	{
		pthread_mutex_unlock(&sim_data->forks[first_fork]);
		return (false);
	}
	pthread_mutex_lock(&sim_data->forks[second_fork]);
	print_status(sim_data, philo->id, "has taken a fork");
	if (check_death(sim_data))
	{
		pthread_mutex_unlock(&sim_data->forks[first_fork]);
		pthread_mutex_unlock(&sim_data->forks[second_fork]);
		return (false);
	}
	print_status(sim_data, philo->id, "is eating");
	pthread_mutex_lock(&sim_data->death_mutex);
	philo->last_meal_time = get_current_time();
	philo->eat_count++;
	pthread_mutex_unlock(&sim_data->death_mutex);
	precise_sleep(sim_data->time_to_eat);
	pthread_mutex_unlock(&sim_data->forks[second_fork]);
	pthread_mutex_unlock(&sim_data->forks[first_fork]);
	return (!check_death(sim_data));
}

void	philosopher_sleep(t_simulation_data *sim_data, t_philosopher *philo)
{
	if (!check_death(sim_data))
	{
		print_status(sim_data, philo->id, "is sleeping");
		precise_sleep(sim_data->time_to_sleep);
	}
}

void	philosopher_think(t_simulation_data *sim_data, t_philosopher *philo)
{
	if (!check_death(sim_data))
	{
		print_status(sim_data, philo->id, "is thinking");
		precise_sleep((sim_data->time_to_die - sim_data->time_to_eat
				- sim_data->time_to_sleep) / 2);
	}
}
