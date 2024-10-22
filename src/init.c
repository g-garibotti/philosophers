/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 10:56:44 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/22 14:00:21 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static bool	parse_arguments(t_program *prog, int argc, char **argv)
{
	prog->philo_count = ft_atoi(argv[1]);
	prog->time_to_die = ft_atoi(argv[2]);
	prog->time_to_eat = ft_atoi(argv[3]);
	prog->time_to_sleep = ft_atoi(argv[4]);
	prog->must_eat_count = -1;
	if (argc == 6)
		prog->must_eat_count = ft_atoi(argv[5]);
	if (prog->philo_count <= 0 || prog->time_to_die <= 0
		|| prog->time_to_eat <= 0 || prog->time_to_sleep <= 0)
		return (false);
	if (argc == 6 && prog->must_eat_count <= 0)
		return (false);
	return (true);
}

static bool	init_mutexes(t_program *program)
{
	int	i;

	program->forks = malloc(sizeof(pthread_mutex_t) * program->philo_count);
	if (!program->forks)
		return (false);
	i = 0;
	while (i < program->philo_count)
	{
		if (pthread_mutex_init(&program->forks[i], NULL) != 0)
			return (false);
		i++;
	}
	if (pthread_mutex_init(&program->death_mutex, NULL) != 0)
		return (false);
	if (pthread_mutex_init(&program->print_mutex, NULL) != 0)
		return (false);
	return (true);
}

static bool	init_philosophers(t_program *program)
{
	int	i;

	program->philos = malloc(sizeof(t_philo) * program->philo_count);
	if (!program->philos)
		return (false);
	i = 0;
	while (i < program->philo_count)
	{
		program->philos[i].id = i + 1;
		program->philos[i].meals_eaten = 0;
		program->philos[i].last_meal_time = get_time();
		program->philos[i].prog = program;
		program->philos[i].left_fork = &program->forks[i];
		program->philos[i].right_fork = &program->forks[(i + 1)
			% program->philo_count];
		i++;
	}
	return (true);
}

bool	init_program(t_program *prog, int argc, char **argv)
{
	prog->someone_died = false;
	prog->start_time = get_time();
	if (!parse_arguments(prog, argc, argv))
		return (false);
	if (!init_mutexes(prog))
		return (false);
	if (!init_philosophers(prog))
		return (false);
	return (true);
}
