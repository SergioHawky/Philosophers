#include "philosophers.h"

int	check_status(t_philos *philos, t_master *monitor)
{
	size_t	time_since_last_meal;
	int		times_eaten;

	pthread_mutex_lock(&philos->times_eaten_mutex);
	times_eaten = philos->times_eaten;
	pthread_mutex_unlock(&philos->times_eaten_mutex);
	pthread_mutex_lock(&philos->time_last_eaten_mutex);
	time_since_last_meal = get_time() - philos->time_last_eaten;
	pthread_mutex_unlock(&philos->time_last_eaten_mutex);
	if (time_since_last_meal > (unsigned int)monitor->vars->time_to_die)
	{
		pthread_mutex_lock(&monitor->vars->shutdown_mutex);
		monitor->vars->shutdown_flag = 1;
		pthread_mutex_unlock(&monitor->vars->shutdown_mutex);
		return (1);
	}
	if (monitor->vars->number_of_times_each_philosopher_must_eat != -1)
		monitor->total_meals += times_eaten;
	return (0);
}

int	check_meal_count(t_master *thread)
{
	if (thread->vars->number_of_times_each_philosopher_must_eat != -1
		&& thread->total_meals >= thread->vars->number_of_philosophers
		* thread->vars->number_of_times_each_philosopher_must_eat)
	{
		pthread_mutex_lock(&thread->vars->shutdown_mutex);
		thread->vars->shutdown_flag = 1;
		pthread_mutex_unlock(&thread->vars->shutdown_mutex);
		return (1);
	}
	return (0);
}

void	print_death(t_philos *thread)
{
	pthread_mutex_lock(&thread->vars->print_mutex);
	printf("%zu %i died\n", get_time() - thread->vars->start_time, thread->id);
	pthread_mutex_unlock(&thread->vars->print_mutex);
}

void	handle_single_philo(t_master *thread)
{
	printf("0 1 has taken a fork\n");
	usleep(thread->vars->time_to_die * 1000);
	printf("%i 1 died\n", thread->vars->time_to_die);
}

void	*monitor(void *arg)
{
	t_master	*thread;
	t_philos	**philos;
	int			i;

	thread = (t_master *)arg;
	philos = thread->philosophers;
	while (thread->vars->start_time > get_time())
		usleep(50);
	if (thread->vars->number_of_philosophers == 1)
		return (handle_single_philo(thread), NULL);
	while (1)
	{
		i = 0;
		usleep(1000);
		thread->total_meals = 0;
		while (i < thread->vars->number_of_philosophers)
		{
			if (check_status(philos[i], thread))
				return (print_death(philos[i]), NULL);
			i++;
		}
		if (check_meal_count(thread))
			break ;
	}
	return (NULL);
}