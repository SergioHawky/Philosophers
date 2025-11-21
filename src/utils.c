#include "philosophers.h"

size_t	get_time(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}

void	print(t_philos *thread, char *msg)
{
	size_t	time;

	pthread_mutex_lock(&thread->vars->print_mutex);
	time = get_time() - thread->vars->start_time;
	if (check_for_death(thread) != 1)
		printf("%zu %i %s\n", time, thread->id, msg);
	pthread_mutex_unlock(&thread->vars->print_mutex);
}

void	free_philosophers(t_vars *vars, t_philos **philos_array)
{
	int	i;

	if (philos_array == NULL)
		return ;
	i = 0;
	while (i < vars->number_of_philosophers)
	{
		free(philos_array[i]);
		i++;
	}
	free(philos_array);
}

void	destroy_mutexes(t_vars *vars, t_philos **philos)
{
	int	i;

	i = 0;
	while (i < vars->number_of_philosophers)
	{
		pthread_mutex_destroy(&vars->mutexes[i]);
		i++;
	}
	free(vars->mutexes);
	pthread_mutex_destroy(&vars->print_mutex);
	pthread_mutex_destroy(&vars->shutdown_mutex);
	if (philos)
	{
		i = 0;
		while (i < vars->number_of_philosophers)
		{
			pthread_mutex_destroy(&philos[i]->time_last_eaten_mutex);
			pthread_mutex_destroy(&philos[i]->times_eaten_mutex);
			i++;
		}
	}
}

int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	num;

	i = 0;
	sign = 1;
	num = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\v'
		|| str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		num = num * 10 + (str[i] - '0');
		i++;
	}
	return (sign * num);
}