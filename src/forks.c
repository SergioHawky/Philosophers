/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 18:31:03 by seilkiv           #+#    #+#             */
/*   Updated: 2026/01/15 18:31:03 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * Locks two forks in a specific order.
 * The locking order is determined externally to reduce deadlock risk.
 * Blocks execution if a fork is already taken.
 */

static void	lock_forks_in_order(t_philo *philo, int first, int second)
{
	pthread_mutex_lock(&philo->data->forks[first]);
	pthread_mutex_lock(&philo->meal_lock);
	philo->last_meal_time = get_current_time_in_ms();
	pthread_mutex_unlock(&philo->meal_lock);
	ft_printmessage(philo->data, philo->id,
		get_current_time_in_ms() - philo->data->start_time, FORK);
	pthread_mutex_lock(&philo->data->forks[second]);
	ft_printmessage(philo->data, philo->id,
		get_current_time_in_ms() - philo->data->start_time, FORK);
}

// Locks the two forks needed for eating, with an order based on philosopher ID to reduce deadlock risk.

void	take_forks(t_philo *philo)
{
	int	left;
	int	right;

	left = philo->id - 1;
	right = philo->id % philo->data->num_philos;
	if (philo->id % 2 == 0)
		lock_forks_in_order(philo, left, right);
	else
		lock_forks_in_order(philo, right, left);
}

// Unlocks the two forks after eating, allowing other philosophers to use them.

void	put_the_forks_down(t_philo *philos)
{
	int	right_fork;
	int	left_fork;

	left_fork = philos->id - 1;
	right_fork = philos->id % philos->data->num_philos;
	pthread_mutex_unlock(&philos->data->forks[left_fork]);
	pthread_mutex_unlock(&philos->data->forks[right_fork]);
}
