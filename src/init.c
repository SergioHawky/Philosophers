/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 16:22:15 by seilkiv           #+#    #+#             */
/*   Updated: 2025/12/15 14:42:07 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_current_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000);
}

t_philo	*init_data(t_data *data, char **argv, int argc)
{
	t_philo	*philos;

	data->num_philos = ft_atol(argv[1]);
	data->time_to_die = ft_atol(argv[2]);
	data->time_to_eat = ft_atol(argv[3]);
	data->time_to_sleep = ft_atol(argv[4]);
	data->start_time = get_current_time_in_ms();
	data->simulation_stop = 0;
	data->finished_philos = 0;
	pthread_mutex_init(&data->write_lock, NULL);
	pthread_mutex_init(&data->stop_lock, NULL);
	if (argc == 6)
		data->meals = ft_atol(argv[5]);
	else
		data->meals = -1;
	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	if (!data->forks)
		return (NULL);
	philos = philo_storage(data);
	return (philos);
}

t_philo	*philo_storage(t_data *data)
{
	t_philo	*philos;
	int		i;

	i = 0;
	philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!philos)
	{
		free(data->forks);
		return (NULL);
	}
	while (i < data->num_philos)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		philos[i].id = i + 1;
		philos[i].data = data;
		philos[i].finished = 0;
		pthread_mutex_init(&philos[i].meal_lock, NULL);
		philos[i].last_meal_time = data->start_time;
		philos[i].eaten = 0;
		philos[i].thread = 0;
		i++;
	}
	return (philos);
}

/* void	print_data(void)
{
	printf("data of the philos:\n\n");
	printf("num of philos: %d\n", prog_data()->num_philos);
	printf("time to die: %d\n", prog_data()->time_to_die);
	printf("time to eat: %d\n", prog_data()->time_to_eat);
	printf("time to sleep: %d\n", prog_data()->time_to_sleep);
	printf("number of meals: %d\n", prog_data()->meals);
	printf("Time since the program was started: %ld\n",
		get_current_time_in_ms() - prog_data()->start_time);
	return ;
} */