/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_actions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 17:02:35 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/24 17:48:52 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

static bool	is_done_eating(t_program *prog)
{
	int		i;
	bool	done;

	done = false;
	if (prog->must_eat_count == -1)
		return (false);
	pthread_mutex_lock(&prog->death_mutex);
	i = 0;
	done = true;
	while (i < prog->philo_count)
	{
		if (prog->philos[i].meals_eaten < prog->must_eat_count)
		{
			done = false;
			break ;
		}
		i++;
	}
	pthread_mutex_unlock(&prog->death_mutex);
	return (done);
}

bool	philosopher_eat(t_philo *philo)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	if (is_simulation_over(philo->prog) || is_done_eating(philo->prog))
		return (false);
	first = philo->left_fork;
	second = philo->right_fork;
	if (philo->right_fork < philo->left_fork)
	{
		first = philo->right_fork;
		second = philo->left_fork;
	}
	pthread_mutex_lock(first);
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(second);
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(&philo->prog->death_mutex);
	philo->last_meal_time = get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->prog->death_mutex);
	print_status(philo, "is eating");
	smart_sleep(philo->prog->time_to_eat);
	pthread_mutex_unlock(second);
	pthread_mutex_unlock(first);
	return (true);
}

void	philosopher_sleep(t_philo *philo)
{
	if (!is_simulation_over(philo->prog) && !is_done_eating(philo->prog))
	{
		print_status(philo, "is sleeping");
		smart_sleep(philo->prog->time_to_sleep);
	}
}

void	philosopher_think(t_philo *philo)
{
	if (!is_simulation_over(philo->prog) && !is_done_eating(philo->prog))
		print_status(philo, "is thinking");
}
