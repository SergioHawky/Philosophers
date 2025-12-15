/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 16:22:23 by seilkiv           #+#    #+#             */
/*   Updated: 2025/11/25 16:22:23 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	free_philo(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < prog_data()->num_philos)
		pthread_mutex_destroy(&prog_data()->forks[i++]);
	pthread_mutex_destroy(&prog_data()->write_lock);
	free(philos);
	free(prog_data()->forks);
}

int	main(int argc, char **argv)
{
	t_philo	*philos;

	if (argc < 5 || argc > 6)
		return (printf("Error: Expected 4 or 5 arguments\n"), 1);
	if (!validate_args(argv, argc))
		return (printf("Error: Arguments are not valid\n"), 1);
	philos = init_data(argv, argc);
	if (prog_data()->num_philos == 1)
	{
		one_philo(philos);
		free_philo(philos);
		return (0);
	}
	start_simulation(philos);
	free_philo(philos);
	return (0);
}

t_data	*prog_data(void)
{
	static t_data	prog_data;

	return (&prog_data);
}
