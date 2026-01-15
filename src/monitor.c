/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 18:31:29 by seilkiv           #+#    #+#             */
/*   Updated: 2026/01/15 18:31:29 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitor_routine(void *arg)
{
	t_philo	*philos;
	int		i;

	philos = (t_philo *)arg;
	while (1)
	{
		pthread_mutex_lock(&philos[0].data->stop_lock);
		if (philos[0].data->simulation_stop)
		{
			pthread_mutex_unlock(&philos[0].data->stop_lock);
			return (NULL);
		}
		pthread_mutex_unlock(&philos[0].data->stop_lock);
		i = 0;
		while (i < philos[0].data->num_philos)
		{
			if (check_philo_death(&philos[i]))
				return (NULL);
			i++;
		}
		usleep(500);
	}
	return (NULL);
}
