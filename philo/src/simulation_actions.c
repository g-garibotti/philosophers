/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_actions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 17:02:35 by ggaribot          #+#    #+#             */
/*   Updated: 2024/11/07 10:20:47 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

bool	philosopher_eat(t_philo *philo)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	if (is_simulation_over(philo->prog))
		return (false);
	if (philo->id % 2 == 0)
	{
		first = philo->right_fork;
		second = philo->left_fork;
	}
	else
	{
		first = philo->left_fork;
		second = philo->right_fork;
	}
	(pthread_mutex_lock(first), print_status(philo, "has taken a fork"));
	(pthread_mutex_lock(second), print_status(philo, "has taken a fork"));
	print_status(philo, "is eating");
	pthread_mutex_lock(&philo->prog->death_mutex);
	philo->last_meal_time = get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->prog->death_mutex);
	smart_sleep(philo->prog->time_to_eat);
	(pthread_mutex_unlock(second), pthread_mutex_unlock(first));
	return (true);
}

void	philosopher_sleep(t_philo *philo)
{
	if (!is_simulation_over(philo->prog))
	{
		print_status(philo, "is sleeping");
		smart_sleep(philo->prog->time_to_sleep);
	}
}

void	philosopher_think(t_philo *philo)
{
	if (!is_simulation_over(philo->prog))
		print_status(philo, "is thinking");
}
