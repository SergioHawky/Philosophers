#include "philosophers.h"

static int	is_valid_input(int argc, char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

void	init_vars(int argc, char **argv, t_vars *vars)
{
	if (!is_valid_input(argc, argv))
	{
		printf("Error: args should be unsigned ints\n");
		exit(1);
	}
	vars->number_of_philosophers = ft_atoi(argv[1]);
	vars->time_to_die = ft_atoi(argv[2]);
	vars->time_to_eat = ft_atoi(argv[3]);
	vars->time_to_sleep = ft_atoi(argv[4]);
	if (vars->number_of_philosophers > PHILOS_MAX)
	{
		printf("Error: number of philosophers should not exceed %i\n",
			PHILOS_MAX);
		exit(1);
	}
	if (argc == 6)
		vars->number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
	else
		vars->number_of_times_each_philosopher_must_eat = -1;
	vars->start_flag = 0;
	vars->start_time = 0;
	vars->shutdown_flag = 0;
}

int	main(int argc, char **argv)
{
	static t_vars	vars;
	static t_master	master_thread;
	t_philos		**philosophers;

	philosophers = NULL;
	if (argc < 5 || argc > 6)
	{
		printf("5 or 6 args required: number_of_philosophers time_to_die ");
		printf("time_to_eat time_to_sleep ");
		printf("number_of_times_each_philosopher_must_eat]\n");
		exit(1);
	}
	else
	{
		init_vars(argc, argv, &vars);
		philosophers = init_philos(&vars);
		init_threads(&vars, philosophers, &master_thread);
		destroy_mutexes(&vars, philosophers);
		free_philosophers(&vars, philosophers);
	}
}
