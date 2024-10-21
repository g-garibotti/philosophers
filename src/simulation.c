/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 12:28:15 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/21 13:29:16 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	*philosopher_routine(void *arg)
{
	t_philosopher		*philo;
	t_simulation_data	*sim_data;

	philo = (t_philosopher *)arg;
	sim_data = philo->sim_data;
	printf("Debug: Philosopher %d starting routine\n", philo->id);
	
	// Add a small delay to allow all threads to start
	usleep(1000);  // 1ms delay

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
	printf("Debug: Philosopher %d ending routine\n", philo->id);
	return (NULL);
}

void	*death_checker(void *arg)
{
	t_simulation_data	*sim_data;
	int					i;

	sim_data = (t_simulation_data *)arg;
	printf("Debug: Death checker started\n");
	while (!sim_data->sim_stop)
	{
		i = 0;
		while (i < sim_data->num_of_philos && !sim_data->sim_stop)
		{
			if (check_death(sim_data, &sim_data->philosophers[i]))
			{
				printf("Debug: Philosopher %d has died\n", i + 1);
				return (NULL);
			}
			i++;
		}
		usleep(100);
	}
	printf("Debug: Death checker ending\n");
	return (NULL);
}

bool	start_simulation(t_simulation_data *sim_data)
{
	int			i;
	pthread_t	death_checker_thread;

	printf("Debug: Starting simulation\n");
	sim_data->start_time = get_current_time();
	i = 0;
	while (i < sim_data->num_of_philos)
	{
		printf("Debug: Creating thread for philosopher %d\n", i + 1);
		if (pthread_create(&sim_data->philosophers[i].thread, NULL,
				philosopher_routine, &sim_data->philosophers[i]) != 0)
		{
			printf("Debug: Failed to create thread for philosopher %d\n", i + 1);
			return (false);
		}
		i++;
	}
	printf("Debug: Creating death checker thread\n");
	if (pthread_create(&death_checker_thread, NULL, death_checker,
			sim_data) != 0)
	{
		printf("Debug: Failed to create death checker thread\n");
		return (false);
	}
	printf("Debug: Waiting for threads to finish\n");
	if (pthread_join(death_checker_thread, NULL) != 0)
	{
		printf("Debug: Failed to join death checker thread\n");
		return (false);
	}
	printf("Debug: Death checker thread joined\n");
	return (true);
}
