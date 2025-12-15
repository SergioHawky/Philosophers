/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 16:22:34 by seilkiv           #+#    #+#             */
/*   Updated: 2025/11/25 16:22:34 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	start_simulation(t_philo *philos)
{
	int	i;

	i = -1;
	while (++i < prog_data()->num_philos)
	{
		if (pthread_create(&philos[i].thread, NULL, philo_routine,
				&philos[i]) != 0)
			return (write(2, "Error creating a thread\n", 25), 1);
	}
	i = -1;
	while (++i < prog_data()->num_philos)
		pthread_join(philos[i].thread, NULL);
	return (0);
}

static int	philo_sleep_think(t_philo *philos)
{
	ft_printmessage(philos->id,
		get_current_time_in_ms() - prog_data()->start_time, SLEEPING);
	death_checker(philos, prog_data()->time_to_sleep);
	ft_printmessage(philos->id,
		get_current_time_in_ms() - prog_data()->start_time, THINKING);
	if (prog_data()->num_philos % 2 != 0)
		death_checker(philos, prog_data()->time_to_sleep);
	return (0);
}

void	*philo_routine(void *args)
{
	t_philo	*philos;
	int		i;

	i = 0;
	philos = (t_philo *)args;
	philos->last_meal_time = get_current_time_in_ms();
	while (1)
	{
		if (!i++ && philos->id % 2 != 0)
			usleep(prog_data()->time_to_eat);
		if (i && philos->id % 2 != 0)
			usleep(500);
		philo_eating(philos);
		if (prog_data()->meals > 0 && prog_data()->meals == philos->eaten)
			break ;
		philo_sleep_think(philos);
		pthread_mutex_lock(&prog_data()->write_lock);
		if (prog_data()->simulation_stop == 1)
		{
			pthread_mutex_unlock(&prog_data()->write_lock);
			break ;
		}
		pthread_mutex_unlock(&prog_data()->write_lock);
	}
	return (NULL);
}

void	take_forks(t_philo *philos)
{
	int	right_fork;
	int	left_fork;

	left_fork = philos->id - 1;
	right_fork = philos->id % prog_data()->num_philos;
	if (philos->id % 2 == 0)
	{
		pthread_mutex_lock(&prog_data()->forks[left_fork]);
		ft_printmessage(philos->id, get_current_time_in_ms()
			- prog_data()->start_time, "has taken a fork");
		pthread_mutex_lock(&prog_data()->forks[right_fork]);
		ft_printmessage(philos->id, get_current_time_in_ms()
			- prog_data()->start_time, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&prog_data()->forks[right_fork]);
		ft_printmessage(philos->id,
			get_current_time_in_ms() - prog_data()->start_time,
			"has taken a fork");
		pthread_mutex_lock(&prog_data()->forks[left_fork]);
		ft_printmessage(philos->id,
			get_current_time_in_ms() - prog_data()->start_time,
			"has taken a fork");
	}
}

void	put_the_forks_down(t_philo	*philos)
{
	int	right_fork;
	int	left_fork;

	left_fork = philos->id - 1;
	right_fork = philos->id % prog_data()->num_philos;
	if (philos->id % 2 == 0)
	{
		pthread_mutex_unlock(&prog_data()->forks[left_fork]);
		pthread_mutex_unlock(&prog_data()->forks[right_fork]);
	}
	else
	{
		pthread_mutex_unlock(&prog_data()->forks[right_fork]);
		pthread_mutex_unlock(&prog_data()->forks[left_fork]);
	}
}
