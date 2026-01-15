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

int	check_philo_death(t_philo *philo)
{
	long	now;

	pthread_mutex_lock(&philo->meal_lock);
	if (philo->finished)
	{
		pthread_mutex_unlock(&philo->meal_lock);
		return (0);
	}
	now = get_current_time_in_ms();
	if (now - philo->last_meal_time < philo->data->time_to_die)
	{
		pthread_mutex_unlock(&philo->meal_lock);
		return (0);
	}
	pthread_mutex_unlock(&philo->meal_lock);
	pthread_mutex_lock(&philo->data->stop_lock);
	if (!philo->data->simulation_stop)
	{
		philo->data->simulation_stop = 1;
		pthread_mutex_lock(&philo->data->write_lock);
		printf("%ld %d %s\n", now - philo->data->start_time, philo->id, DIED);
		pthread_mutex_unlock(&philo->data->write_lock);
	}
	pthread_mutex_unlock(&philo->data->stop_lock);
	return (1);
}

void	philo_eating(t_philo *philo)
{
	long	now;

	pthread_mutex_lock(&philo->meal_lock);
	now = get_current_time_in_ms();
	if (now - philo->last_meal_time >= philo->data->time_to_die)
	{
		pthread_mutex_unlock(&philo->meal_lock);
		return ;
	}
	pthread_mutex_unlock(&philo->meal_lock);
	take_forks(philo);
	pthread_mutex_lock(&philo->meal_lock);
	philo->last_meal_time = get_current_time_in_ms();
	pthread_mutex_unlock(&philo->meal_lock);
	ft_printmessage(philo->data, philo->id,
		get_current_time_in_ms() - philo->data->start_time, EATING);
	usleep(philo->data->time_to_eat * 1000);
	pthread_mutex_lock(&philo->meal_lock);
	philo->eaten++;
	pthread_mutex_unlock(&philo->meal_lock);
	put_the_forks_down(philo);
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

int	simulation_should_stop(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->stop_lock);
	if (philo->data->simulation_stop)
	{
		pthread_mutex_unlock(&philo->data->stop_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->stop_lock);
	return (0);
}

int	check_and_finish_meals(t_philo *philo)
{
	if (philo->data->meals > 0 && philo->eaten >= philo->data->meals)
	{
		pthread_mutex_lock(&philo->meal_lock);
		philo->finished = 1;
		pthread_mutex_unlock(&philo->meal_lock);
		pthread_mutex_lock(&philo->data->stop_lock);
		philo->data->finished_philos++;
		if (philo->data->finished_philos == philo->data->num_philos)
			philo->data->simulation_stop = 1;
		pthread_mutex_unlock(&philo->data->stop_lock);
		return (1);
	}
	return (0);
}

/*pthread_mutex_lock(&philo->data->write_lock);
		printf("[DEBUG] philo %d FINISHED meals (%d)\n",
			philo->id, philo->eaten);
		pthread_mutex_unlock(&philo->data->write_lock);*/