/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 16:22:15 by seilkiv           #+#    #+#             */
/*   Updated: 2025/11/25 16:22:15 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/// @brief main function to data initialization
/// @param argv 
/// @param argc 
/// @return array of philosophers
t_philo	*init_data(char **argv, int argc)
{
	t_philo	*philos;

	prog_data()->num_philos = ft_atol(argv[1]);
	prog_data()->time_to_die = ft_atol(argv[2]);
	prog_data()->time_to_eat = ft_atol(argv[3]);
	prog_data()->time_to_sleep = ft_atol(argv[4]);
	prog_data()->start_time = get_current_time_in_ms();
	prog_data()->simulation_stop = 0;
	if (argc == 6)
		prog_data()->meals = ft_atol(argv[5]);
	else
		prog_data()->meals = -1;
	prog_data()->forks = malloc(sizeof(pthread_mutex_t)
		* prog_data()->num_philos);
	if (!prog_data()->forks)
		return (NULL);
	philos = philo_storage();
	return (philos);
}

/// @brief function to get the time precisely 
/// @return
long	get_current_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000);
}

/// @brief function to store all the data inside the philosophers
t_philo	*philo_storage(void)
{
	t_philo	*philos;
	int		i;

	i = 0;
	philos = malloc(sizeof(t_philo) * prog_data()->num_philos);
	if (!philos)
	{
		free(prog_data()->forks);
		return (NULL);
	}
	while (i < prog_data()->num_philos)
	{
		pthread_mutex_init(&prog_data()->forks[i], NULL);
		philos[i].id = i + 1;
		philos[i].data = prog_data();
		philos[i].last_meal_time = prog_data()->start_time;
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