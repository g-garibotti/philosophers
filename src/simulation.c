/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 12:28:15 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/23 16:30:54 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// Checks if simulation should stop (someone died or all ate enough)
bool	is_simulation_over(t_program *prog)
{
	bool	should_stop;
	int		i;
	int		finished_count;

	pthread_mutex_lock(&prog->death_mutex);
	should_stop = prog->someone_died;
	// Check if all philosophers ate enough times
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

// The main routine each philosopher follows
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

// Handles taking forks and eating
bool	philosopher_eat(t_philo *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	// Always take the lower numbered fork first
	if (philo->left_fork < philo->right_fork)
	{
		first_fork = philo->left_fork;
		second_fork = philo->right_fork;
	}
	else
	{
		first_fork = philo->right_fork;
		second_fork = philo->left_fork;
	}
	if (is_simulation_over(philo->prog))
		return (false);
	pthread_mutex_lock(first_fork);
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(second_fork);
	print_status(philo, "has taken a fork");
	print_status(philo, "is eating");
	pthread_mutex_lock(&philo->prog->death_mutex);
	philo->last_meal_time = get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->prog->death_mutex);
	smart_sleep(philo->prog->time_to_eat);
	pthread_mutex_unlock(second_fork);
	pthread_mutex_unlock(first_fork);
	return (!is_simulation_over(philo->prog));
}

// Sleeping phase
void	philosopher_sleep(t_philo *philo)
{
	if (!is_simulation_over(philo->prog))
	{
		print_status(philo, "is sleeping");
		smart_sleep(philo->prog->time_to_sleep);
	}
}

// Thinking phase
void	philosopher_think(t_philo *philo)
{
	if (!is_simulation_over(philo->prog))
		print_status(philo, "is thinking");
}

// Starts all threads and manages them
bool	start_simulation(t_program *prog)
{
	pthread_t	*threads;
	pthread_t	monitor;
	int			i;

	threads = malloc(sizeof(pthread_t) * prog->philo_count);
	if (!threads)
		return (false);
	prog->start_time = get_time();
	// Create philosopher threads
	i = 0;
	while (i < prog->philo_count)
	{
		if (pthread_create(&threads[i], NULL, philosopher_routine,
				&prog->philos[i]) != 0)
			return (false);
		i++;
	}
	// Create monitor thread
	if (pthread_create(&monitor, NULL, death_monitor, prog) != 0)
		return (false);
	// Wait for monitor to finish
	pthread_join(monitor, NULL);
	// Wait for all philosophers
	i = 0;
	while (i < prog->philo_count)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	free(threads);
	return (true);
}
