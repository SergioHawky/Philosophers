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

#include "philosophers.h"

void	error_and_exit(char *message)
{
	printf("%s\n", message);
	exit(1);
}

t_philos	**init_philos(t_vars *vars)
{
	t_philos	**philos_array;
	int			i;
	t_philos	*curr_philosopher;

	i = 0;
	philos_array = malloc(sizeof(t_philos **) * vars->number_of_philosophers);
	if (philos_array == NULL)
		error_and_exit("Malloc error in init_philos");
	while (i < vars->number_of_philosophers)
	{
		curr_philosopher = malloc(sizeof(t_philos));
		if (curr_philosopher == NULL)
		{
			free_philosophers(vars, philos_array);
			error_and_exit("Malloc error in init_philos");
		}
		curr_philosopher->id = i + 1;
		curr_philosopher->time_last_eaten = 0;
		curr_philosopher->times_eaten = 0;
		curr_philosopher->death_flag = 0;
		curr_philosopher->vars = vars;
		philos_array[i] = curr_philosopher;
		i++;
	}
	return (philos_array);
}

void	create_forks(t_philos **philos_array, t_vars *vars)
{
	int	i;

	vars->mutexes = malloc(sizeof(pthread_mutex_t)
			* vars->number_of_philosophers);
	if (!vars->mutexes)
	{
		free_philosophers(vars, philos_array);
		error_and_exit("Malloc error in create_forks");
	}
	i = 0;
	while (i < vars->number_of_philosophers)
	{
		pthread_mutex_init(&vars->mutexes[i], NULL);
		philos_array[i]->left_fork = i;
		philos_array[i]->right_fork = (i + 1) % vars->number_of_philosophers;
		i++;
	}
}

void	init_mutexes(t_philos **philos_array, t_vars *vars)
{
	int	i;

	create_forks(philos_array, vars);
	pthread_mutex_init(&vars->print_mutex, NULL);
	pthread_mutex_init(&vars->shutdown_mutex, NULL);
	i = 0;
	while (i < vars->number_of_philosophers)
	{
		pthread_mutex_init(&philos_array[i]->time_last_eaten_mutex, NULL);
		pthread_mutex_init(&philos_array[i]->times_eaten_mutex, NULL);
		i++;
	}
}

void	init_threads(t_vars *vars, t_philos **philos_array, t_master *master)
{
	int	i;

	i = 0;
	init_mutexes(philos_array, vars);
	master->philosophers = philos_array;
	master->vars = vars;
	vars->start_time = get_time() + 100;
	vars->start_flag = 1;
	while (i < vars->number_of_philosophers)
	{
		pthread_create(&philos_array[i]->thread, NULL, routine,
			philos_array[i]);
		i++;
	}
	pthread_create(&master->master_thread, NULL, monitor, master);
	pthread_join(master->master_thread, NULL);
	i = 0;
	while (i < vars->number_of_philosophers)
	{
		pthread_join(philos_array[i]->thread, NULL);
		i++;
	}
}