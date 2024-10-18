/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 22:23:14 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/18 00:50:31 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

static int	check_args(int argc, char **argv)
{
	int	i;
	int	j;

	if (argc < 5 || argc > 6)
		return (ERROR);
	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (ERROR);
			j++;
		}
		if (ft_atoi(argv[i]) <= 0)
			return (ERROR);
		i++;
	}
	return (SUCCESS);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (check_args(argc, argv) != SUCCESS)
		error_exit("Invalid arguments", &data);
	if (init_data(&data, argc, argv) != SUCCESS)
		error_exit("Error initializing data", &data);
	if (create_threads(&data) != SUCCESS)
		error_exit("Error creating threads", &data);
	check_death(&data);
	if (join_threads(&data) != SUCCESS)
		error_exit("Error joining threads", &data);
	cleanup_data(&data);
	return (SUCCESS);
}
