/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggaribot <ggaribot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 12:12:28 by ggaribot          #+#    #+#             */
/*   Updated: 2024/10/22 14:10:57 by ggaribot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
** Gets current time in milliseconds
** Returns the time since Epoch (Jan 1, 1970) in milliseconds
*/
long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	// Convert seconds to milliseconds and add microseconds converted to milliseconds
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/*
** More precise sleep function than usleep
** Keeps checking time until desired time has passed
** ms: time to sleep in milliseconds
*/
void	smart_sleep(long long ms)
{
	long long	start;
	long long	current;

	start = get_time();
	while (1)
	{
		current = get_time();
		if (current - start >= ms)
			break ;
		// Sleep in small intervals to not hog CPU
		usleep(500);
	}
}

/*
** Prints philosopher status with timestamp
** Format: timestamp_in_ms X status
** Uses mutex to prevent messages from mixing
*/
void	print_status(t_philo *philo, char *status)
{
	long long	timestamp;

	pthread_mutex_lock(&philo->prog->print_mutex);
	if (!is_simulation_over(philo->prog))
	{
		timestamp = get_time() - philo->prog->start_time;
		printf("%lld %d %s\n", timestamp, philo->id, status);
	}
	pthread_mutex_unlock(&philo->prog->print_mutex);
}

/*
** Simple atoi function that converts string to integer
** Returns the converted number
*/
int	ft_atoi(const char *str)
{
	int	result;
	int	sign;

	result = 0;
	sign = 1;
	// Skip whitespace
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	// Handle sign
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	// Convert digits
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	return (sign * result);
}

/*
** Checks if a string is a valid number
** Returns true if string contains only digits (and optional + sign)
*/
bool	is_number(char *str)
{
	int	i;

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

/*
** Validates that a string represents a positive integer within bounds
** Returns true if string is valid positive int and not bigger than INT_MAX
*/
bool	is_valid_positive_int(char *str)
{
	long long	num;
	int			i;

	if (!is_number(str))
		return (false);
	num = 0;
	i = 0;
	if (str[i] == '+')
		i++;
	while (str[i])
	{
		num = num * 10 + (str[i] - '0');
		if (num > INT_MAX)
			return (false);
		i++;
	}
	return (num > 0);
}
