/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 16:22:34 by seilkiv           #+#    #+#             */
/*   Updated: 2025/12/15 14:42:07 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * Creates philosopher threads and a monitor thread.
 * Waits for all philosopher threads to finish,
 * then joins the monitor thread before exiting.
 */

int	start_simulation(t_philo *philos)
{
	int			i;
	pthread_t	monitor;

	i = -1;
	while (++i < philos[0].data->num_philos)
	{
		if (pthread_create(&philos[i].thread, NULL,
				philo_routine, &philos[i]) != 0)
			return (write(2, "Error creating a thread\n", 25), 1);
	}
	if (pthread_create(&monitor, NULL, monitor_routine, philos) != 0)
		return (write(2, "Error creating monitor\n", 23), 1);
	i = -1;
	while (++i < philos[0].data->num_philos)
		pthread_join(philos[i].thread, NULL);
	pthread_join(monitor, NULL);
	return (0);
}

/*
 * Main routine executed by each philosopher thread.
 * Handles the philosopher life cycle:
 * thinking, taking forks, eating, sleeping.
 * Stops when simulation ends or required meals are reached.
 */

void	*philo_routine(void *args)
{
	t_philo	*philo;

	philo = (t_philo *)args;
	if (philo->id % 2)
		usleep(philo->data->time_to_eat * 1000 / 2);  // Stagger start times for odd philosophers to reduce contention
	while (1)
	{
		if (simulation_should_stop(philo))
			break ;
		philo_eating(philo);
		if (check_and_finish_meals(philo))
			break ;
		ft_printmessage(philo->data, philo->id,
			get_current_time_in_ms() - philo->data->start_time, SLEEPING);
		usleep(philo->data->time_to_sleep * 1000);
		ft_printmessage(philo->data, philo->id,
			get_current_time_in_ms() - philo->data->start_time, THINKING);
	}
	return (NULL);
}
