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

void	*philo_routine(void *args)
{
	t_philo	*philo;

	philo = (t_philo *)args;
	if (philo->id % 2)
		usleep(philo->data->time_to_eat * 1000 / 2);

	while (1)
	{
		pthread_mutex_lock(&philo->data->stop_lock);
		if (philo->data->simulation_stop)
		{
			pthread_mutex_unlock(&philo->data->stop_lock);
			break;
		}
		pthread_mutex_unlock(&philo->data->stop_lock);

		if (philo->id % 2 == 0)
			usleep(500); // 🔴 QUEBRA PADRÃO FIXO

		philo_eating(philo);

		if (philo->data->meals > 0 && philo->eaten >= philo->data->meals)
		{
			pthread_mutex_lock(&philo->meal_lock);
			philo->finished = 1;
			pthread_mutex_unlock(&philo->meal_lock);
			
			pthread_mutex_lock(&philo->data->write_lock);
			printf("[DEBUG] philo %d FINISHED meals (%d)\n",
				philo->id, philo->eaten);
			pthread_mutex_unlock(&philo->data->write_lock);

			pthread_mutex_lock(&philo->data->stop_lock);
			philo->data->finished_philos++;
			if (philo->data->finished_philos == philo->data->num_philos)
				philo->data->simulation_stop = 1;
			pthread_mutex_unlock(&philo->data->stop_lock);
			break;
		}
		ft_printmessage(philo->data, philo->id,
			get_current_time_in_ms() - philo->data->start_time, SLEEPING);
		usleep(philo->data->time_to_sleep * 1000);

		ft_printmessage(philo->data, philo->id,
			get_current_time_in_ms() - philo->data->start_time, THINKING);
	}
	return (NULL);
}

void	take_forks(t_philo *philo)
{
	int	left;
	int	right;

	left = philo->id - 1;
	right = philo->id % philo->data->num_philos;

	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->data->forks[left]);

		pthread_mutex_lock(&philo->meal_lock);
		philo->last_meal_time = get_current_time_in_ms();
		pthread_mutex_unlock(&philo->meal_lock);

		ft_printmessage(philo->data, philo->id,
			get_current_time_in_ms() - philo->data->start_time, FORK);

		pthread_mutex_lock(&philo->data->forks[right]);
		ft_printmessage(philo->data, philo->id,
			get_current_time_in_ms() - philo->data->start_time, FORK);
	}
	else
	{
		pthread_mutex_lock(&philo->data->forks[right]);

		pthread_mutex_lock(&philo->meal_lock);
		philo->last_meal_time = get_current_time_in_ms();
		pthread_mutex_unlock(&philo->meal_lock);

		ft_printmessage(philo->data, philo->id,
			get_current_time_in_ms() - philo->data->start_time, FORK);

		pthread_mutex_lock(&philo->data->forks[left]);
		ft_printmessage(philo->data, philo->id,
			get_current_time_in_ms() - philo->data->start_time, FORK);
	}
}

void	put_the_forks_down(t_philo *philos)
{
	int	right_fork;
	int	left_fork;

	left_fork = philos->id - 1;
	right_fork = philos->id % philos->data->num_philos;
	pthread_mutex_unlock(&philos->data->forks[left_fork]);
	pthread_mutex_unlock(&philos->data->forks[right_fork]);
}

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
