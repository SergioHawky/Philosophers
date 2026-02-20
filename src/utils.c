/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 16:22:40 by seilkiv           #+#    #+#             */
/*   Updated: 2025/12/15 14:42:07 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// Checks if the given string represents a valid number (optional leading + or -)

bool	ft_isnbr(char *nbr)
{
	int	i;

	i = 0;
	if (nbr[i] == '+' || nbr[i] == '-')
		i++;
	if (!nbr[i])
		return (false);
	while (nbr[i])
	{
		if (nbr[i] < '0' || nbr[i] > '9')
			return (false);
		i++;
	}
	return (true);
}

// Validates that all provided arguments are numeric strings.

bool	ft_valid(char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		if (!ft_isnbr(argv[i]))
			return (false);
		i++;
	}
	return (true);
}

/*
 * Converts a numeric string to a long.
 * Returns -1 if sign is detected.
 * Performs basic overflow protection.
 */

long	ft_atol(const char *nptr)
{
	int		count;
	int		i;
	long	num;

	num = 0;
	i = 0;
	count = 1;
	while ((nptr[i] >= '\t' && nptr[i] <= '\r') || nptr[i] == ' ')
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
		return (-1);
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		num = num * 10 + (nptr[i] - 48);
		i++;
		if (num > INT_MAX || num < INT_MIN)
			return ((long)INT_MAX + 1);
	}
	return (num * count);
}

/*
 * Validates argument count and ensures all values
 * are within acceptable limits defined for the simulation.
 */

bool	validate_args(char **argv, int argc)
{
	if (!ft_valid(&argv[1]))
		return (false);
	if ((ft_atol(argv[1]) < 0 || ft_atol(argv[1]) > 200))
		return (false);
	if ((ft_atol(argv[2]) < 0 || ft_atol(argv[2]) > 1000))
		return (false);
	if ((ft_atol(argv[3]) < 0 || ft_atol(argv[3]) > 1000))
		return (false);
	if ((ft_atol(argv[4]) < 0 || ft_atol(argv[4]) > 1000))
		return (false);
	if (argc == 6)
		if ((ft_atol(argv[5]) < 0 || ft_atol(argv[5]) > 1000))
			return (false);
	return (true);
}

/*
 * Prints philosopher state messages in a thread-safe way.
 * Ensures no overlapping logs and stops printing after simulation ends.
 */

void	ft_printmessage(t_data *data, int philo_id, long time, char *str)
{
	pthread_mutex_lock(&data->write_lock);
	if (!data->simulation_stop)
		printf("%ld %d %s\n", time, philo_id, str);
	pthread_mutex_unlock(&data->write_lock);
}
