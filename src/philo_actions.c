/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 00:18:47 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/18 00:26:17 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	take_forks(t_philosopher *philo)
{
	t_data	*data;

	data = philo->data;
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&data->forks[philo->right_fork]);
		print_status(data, philo->id, "has taken a fork");
		pthread_mutex_lock(&data->forks[philo->left_fork]);
		print_status(data, philo->id, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&data->forks[philo->left_fork]);
		print_status(data, philo->id, "has taken a fork");
		pthread_mutex_lock(&data->forks[philo->right_fork]);
		print_status(data, philo->id, "has taken a fork");
	}
}

static void	put_down_forks(t_philosopher *philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_unlock(&data->forks[philo->left_fork]);
	pthread_mutex_unlock(&data->forks[philo->right_fork]);
}

void	philo_eat(t_philosopher *philo)
{
	t_data	*data;

	data = philo->data;
	take_forks(philo);
	print_status(data, philo->id, "is eating");
	philo->last_meal_time = get_time();
	smart_sleep(data->time_to_eat, data);
	philo->meals_eaten++;
	put_down_forks(philo);
}

void	*philosopher(void *arg)
{
	t_philosopher	*philo;
	t_data			*data;

	philo = (t_philosopher *)arg;
	data = philo->data;
	if (philo->id % 2 == 0)
		usleep(1000);
	while (!data->someone_died && (data->meals_to_eat == -1
			|| philo->meals_eaten < data->meals_to_eat))
	{
		philo_eat(philo);
		philo_sleep(philo);
		philo_think(philo);
	}
	return (NULL);
}
