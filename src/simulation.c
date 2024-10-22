/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 12:28:15 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/22 14:00:28 by ggaribot         ###   ########.fr       */
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
static void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	// If only one philosopher, handle special case
	if (philo->prog->philo_count == 1)
	{
		print_status(philo, "has taken a fork");
		smart_sleep(philo->prog->time_to_die);
		return (NULL);
	}
	// Even numbered philosophers wait a bit to prevent deadlock
	if (philo->id % 2 == 0)
		usleep(1000);
	while (!is_simulation_over(philo->prog))
	{
		philosopher_eat(philo);
		philosopher_sleep(philo);
		philosopher_think(philo);
	}
	return (NULL);
}

// Handles taking forks and eating
static bool	philosopher_eat(t_philo *philo)
{
	// Take left fork first
	pthread_mutex_lock(philo->left_fork);
	print_status(philo, "has taken a fork");
	// Take right fork
	pthread_mutex_lock(philo->right_fork);
	print_status(philo, "has taken a fork");
	// Start eating
	print_status(philo, "is eating");
	pthread_mutex_lock(&philo->prog->death_mutex);
	philo->last_meal_time = get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->prog->death_mutex);
	// Eat for specified time
	smart_sleep(philo->prog->time_to_eat);
	// Put down forks
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	return (!is_simulation_over(philo->prog));
}

// Sleeping phase
static void	philosopher_sleep(t_philo *philo)
{
	if (!is_simulation_over(philo->prog))
	{
		print_status(philo, "is sleeping");
		smart_sleep(philo->prog->time_to_sleep);
	}
}

// Thinking phase
static void	philosopher_think(t_philo *philo)
{
	if (!is_simulation_over(philo->prog))
		print_status(philo, "is thinking");
}

// Monitor thread that checks if any philosopher died
static void	*death_monitor(void *arg)
{
	t_program	*prog;
	int			i;
	long long	time_since_meal;

	prog = (t_program *)arg;
	while (!is_simulation_over(prog))
	{
		i = 0;
		while (i < prog->philo_count)
		{
			pthread_mutex_lock(&prog->death_mutex);
			time_since_meal = get_time() - prog->philos[i].last_meal_time;
			if (time_since_meal > prog->time_to_die)
			{
				prog->someone_died = true;
				print_status(&prog->philos[i], "died");
				pthread_mutex_unlock(&prog->death_mutex);
				return (NULL);
			}
			pthread_mutex_unlock(&prog->death_mutex);
			i++;
		}
		usleep(1000);
	}
	return (NULL);
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
