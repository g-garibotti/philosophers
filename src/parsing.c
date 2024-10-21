/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:51:14 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/21 14:55:08 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

static bool is_valid_number(const char *str)
{
    int i;

    i = 0;
    if (str[i] == '+')
        i++;
    if (!str[i])
        return (false);
    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return (false);
        i++;
    }
    return (true);
}

static long long ft_atoll(const char *str)
{
    long long result;
    int sign;

    result = 0;
    sign = 1;
    while ((*str <= 13 && *str >= 9) || *str == ' ')
        str++;
    if (*str == '-' || *str == '+')
    {
        if (*str == '-')
            sign = -1;
        str++;
    }
    while (*str >= '0' && *str <= '9')
    {
        result = result * 10 + (*str - '0');
        str++;
    }
    return (sign * result);
}

static bool is_valid_int(const char *str)
{
    long long num;

    if (!is_valid_number(str))
        return (false);
    num = ft_atoll(str);
    return (num >= 0 && num <= INT_MAX);
}

bool validate_arguments(int argc, char **argv)
{
    int i;

    if (argc != 5 && argc != 6)
        return (false);
    i = 1;
    while (i < argc)
    {
        if (!is_valid_int(argv[i]))
            return (false);
        i++;
    }
    return (true);
}

void set_simulation_params(t_simulation_data *sim_data, int argc, char **argv)
{
    sim_data->num_of_philos = ft_atoi(argv[1]);
    sim_data->time_to_die = ft_atoi(argv[2]);
    sim_data->time_to_eat = ft_atoi(argv[3]);
    sim_data->time_to_sleep = ft_atoi(argv[4]);
    if (argc == 6)
        sim_data->num_times_to_eat = ft_atoi(argv[5]);
    else
        sim_data->num_times_to_eat = -1;
    sim_data->sim_stop = false;
}
