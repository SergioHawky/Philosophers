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

bool	ft_valid(int argc, char **argv)
{
	int		i;
	int		j;

	i = 0;
	while (i < argc - 1)
	{
		j = 0;
		while (argv[j])
		{
			if (!ft_isnbr(argv[j]))
				return (false);
			j++;
		}
		i++;
	}
	return (true);
}

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

bool	validate_args(char **argv, int argc)
{
	if (!ft_valid(argc, &argv[1]))
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

void	ft_printmessage(t_data *data, int philo_id, long time, char *str)
{
	pthread_mutex_lock(&data->write_lock);
	if (!data->simulation_stop)
		printf("%ld %d %s\n", time, philo_id, str);
	pthread_mutex_unlock(&data->write_lock);
}
