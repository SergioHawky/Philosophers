/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 16:22:29 by seilkiv           #+#    #+#             */
/*   Updated: 2025/11/25 16:22:29 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	death_checker(t_philo *philos, long state)
{
	long	time_mark;

	pthread_mutex_lock(&prog_data()->write_lock);
	time_mark = get_current_time_in_ms() - philos->last_meal_time;
	if (time_mark + state > prog_data()->time_to_die
		&& prog_data()->simulation_stop == 0)
	{
		if (prog_data()->time_to_die - time_mark > 0)
			usleep((prog_data()->time_to_die - time_mark) * 1000);
		printf("%ld %d %s\n", get_current_time_in_ms()
			- prog_data()->start_time, philos->id, DIED);
		prog_data()->simulation_stop = 1;
		pthread_mutex_unlock(&prog_data()->write_lock);
		return (true);
	}
	else
	{
		pthread_mutex_unlock(&prog_data()->write_lock);
		usleep(state * 1000);
	}
	return (false);
}

void	philo_eating(t_philo *philos)
{
	take_forks(philos);
	ft_printmessage(philos->id, get_current_time_in_ms()
		- prog_data()->start_time, EATING);
	death_checker(philos, prog_data()->time_to_eat);
	philos->last_meal_time = get_current_time_in_ms();
	philos->eaten++;
	put_the_forks_down(philos);
}

void	one_philo(t_philo *philos)
{
	pthread_mutex_lock(&prog_data()->forks[philos->id - 1]);
	ft_printmessage(philos->id, get_current_time_in_ms()
		- prog_data()->start_time, LEFT_FORK);
	pthread_mutex_unlock(&prog_data()->forks[philos->id - 1]);
	usleep(prog_data()->time_to_die * 1000);
	ft_printmessage(philos->id, get_current_time_in_ms()
		- prog_data()->start_time, DIED);
	return ;
}
