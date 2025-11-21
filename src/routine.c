#include "philosophers.h"

int	check_for_death(t_philos *thread)
{
	pthread_mutex_lock(&thread->vars->shutdown_mutex);
	thread->death_flag = thread->vars->shutdown_flag;
	pthread_mutex_unlock(&thread->vars->shutdown_mutex);
	if (thread->death_flag == 1)
		return (1);
	return (0);
}

void	go_to_sleep(t_philos *thread)
{
	size_t	end_time;

	end_time = get_time() + thread->vars->time_to_sleep;
	print(thread, "is sleeping");
	while (get_time() < end_time && check_for_death(thread) == 0)
	{
		usleep(500);
	}
}

void	take_forks(t_philos *thread)
{
	int	first;
	int	second;

	if (thread->id % 2 == 0)
	{
		first = thread->left_fork;
		second = thread->right_fork;
	}
	else
	{
		first = thread->right_fork;
		second = thread->left_fork;
		usleep(500);
	}
	pthread_mutex_lock(&thread->vars->mutexes[first]);
	print(thread, "has taken a fork");
	pthread_mutex_lock(&thread->vars->mutexes[second]);
	print(thread, "has taken a fork");
}

void	eat(t_philos *thread)
{
	size_t	end_time;

	pthread_mutex_lock(&thread->time_last_eaten_mutex);
	thread->time_last_eaten = get_time();
	end_time = thread->time_last_eaten + thread->vars->time_to_eat;
	pthread_mutex_unlock(&thread->time_last_eaten_mutex);
	print(thread, "is eating");
	while (get_time() < end_time && check_for_death(thread) == 0)
	{
		usleep(500);
	}
	pthread_mutex_unlock(&thread->vars->mutexes[thread->left_fork]);
	pthread_mutex_unlock(&thread->vars->mutexes[thread->right_fork]);
	pthread_mutex_lock(&thread->times_eaten_mutex);
	thread->times_eaten++;
	pthread_mutex_unlock(&thread->times_eaten_mutex);
}

void	*routine(void *arg)
{
	t_philos	*thread;
	size_t		start;

	thread = (t_philos *)arg;
	if (thread->vars->number_of_philosophers == 1)
		return (NULL);
	start = thread->vars->start_time;
	pthread_mutex_lock(&thread->time_last_eaten_mutex);
	thread->time_last_eaten = start;
	pthread_mutex_unlock(&thread->time_last_eaten_mutex);
	while (start > get_time())
		usleep(500);
	if (thread->vars->number_of_philosophers % 2 == 1 && thread->id == 1)
		usleep(500);
	while (thread->death_flag == 0)
	{
		take_forks(thread);
		eat(thread);
		go_to_sleep(thread);
		print(thread, "is thinking");
		usleep(50);
	}
	return (NULL);
}