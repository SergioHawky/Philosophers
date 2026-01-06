/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 16:22:29 by seilkiv           #+#    #+#             */
/*   Updated: 2025/12/15 14:42:07 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	death_checker(t_philo *philos, long state)
{
	long	time_mark;

	pthread_mutex_lock(&philos->data->write_lock);
	time_mark = get_current_time_in_ms() - philos->last_meal_time;
	if (time_mark + state > philos->data->time_to_die
		&& philos->data->simulation_stop == 0)
	{
		if (philos->data->time_to_die - time_mark > 0)
			usleep((philos->data->time_to_die - time_mark) * 1000);
		printf("%ld %d %s\n",
			get_current_time_in_ms() - philos->data->start_time,
			philos->id, DIED);
		philos->data->simulation_stop = 1;
		pthread_mutex_unlock(&philos->data->write_lock);
		return (true);
	}
	pthread_mutex_unlock(&philos->data->write_lock);
	usleep(state * 1000);
	return (false);
}

void	philo_eating(t_philo *philos)
{
	take_forks(philos);
	ft_printmessage(philos->data, philos->id,
		get_current_time_in_ms() - philos->data->start_time, EATING);
	death_checker(philos, philos->data->time_to_eat);
	philos->last_meal_time = get_current_time_in_ms();
	philos->eaten++;
	put_the_forks_down(philos);
}

void	one_philo(t_philo *philos)
{
	pthread_mutex_lock(&philos->data->forks[philos->id - 1]);
	ft_printmessage(philos->data, philos->id,
		get_current_time_in_ms() - philos->data->start_time, LEFT_FORK);
	pthread_mutex_unlock(&philos->data->forks[philos->id - 1]);
	usleep(philos->data->time_to_die * 1000);
	ft_printmessage(philos->data, philos->id,
		get_current_time_in_ms() - philos->data->start_time, DIED);
}
