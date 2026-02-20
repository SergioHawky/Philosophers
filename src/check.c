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

/*
 * Checks if a philosopher has exceeded time_to_die.
 * If so, safely stops the simulation and prints death message.
 * Protected with meal_lock and stop_lock to avoid race conditions.
 */

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

/*
 * Handles the eating phase:
 * acquires forks, updates last_meal_time,
 * prints eating state, waits time_to_eat,
 * increments meal count and releases forks.
 */

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

// Handles the case when there is only one philosopher in the simulation.

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

/*
 * Checks if the simulation has been marked to stop.
 * Protected by stop_lock to ensure thread-safe access.
 */

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

/*
 * Checks if philosopher reached required meal count.
 * Marks philosopher as finished and updates global counter.
 * Stops simulation when all philosophers have finished.
 */

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