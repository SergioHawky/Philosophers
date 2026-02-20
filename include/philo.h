/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 16:22:47 by seilkiv           #+#    #+#             */
/*   Updated: 2025/12/15 14:42:07 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <stdbool.h>
# include <sys/time.h>

# define RIGHT_FORK    "has taken right fork"
# define LEFT_FORK     "has taken left fork"
# define FORK          "has taken a fork"
# define EATING        "is eating"
# define SLEEPING      "is sleeping"
# define THINKING      "is thinking"
# define DIED          "died"

typedef struct s_philo
{
	int				id;								// Philosopher ID (1 to num_philos)
	long			last_meal_time;					// Timestamp of the last meal in milliseconds
	int				eaten;							// Number of times the philosopher has eaten		
	int				finished;						// Flag to indicate if the philosopher has finished eating required meals
	pthread_t		thread;							// Thread for the philosopher's routine
	pthread_mutex_t	meal_lock;						// Mutex to protect access to last_meal_time and eaten
	struct s_data	*data;							// Pointer to shared data structure
}	t_philo;

typedef struct s_data
{
	int				num_philos;						// Total number of philosophers
	int				time_to_die;					// Time in milliseconds before a philosopher dies if they don't eat
	int				time_to_eat;					// Time in milliseconds it takes for a philosopher to eat
	int				time_to_sleep;					// Time in milliseconds a philosopher spends sleeping
	int				meals;							// Number of times each philosopher must eat (optional, 0 if not specified)
	pthread_mutex_t	*forks;							// Array of mutexes representing the forks (size num_philos)
	pthread_mutex_t	write_lock;						// Mutex to protect writing to the console
	pthread_mutex_t	stop_lock;						// Mutex to protect access to simulation_stop and finished_philos
	int				finished_philos;				// Counter for philosophers who have finished eating required meals
	long			start_time;						// Timestamp when the simulation started in milliseconds
	int				simulation_stop;				// Flag to indicate if the simulation should stop (1 if a philosopher has died or all have finished eating)
}	t_data;

//utils
bool	validate_args(char **argv, int argc);
long	ft_atol(const char *nptr);
bool	ft_valid(char **argv);
bool	ft_isnbr(char *nbr);
void	ft_printmessage(t_data *data, int philo_id, long time, char *str);
long	get_current_time_in_ms(void);

//init
t_philo	*philo_storage(t_data *data);
t_philo	*init_data(t_data *data, char **argv, int argc);

//philo routine
void	*philo_routine(void *args);
int		start_simulation(t_philo *philos);
void	philo_eating(t_philo *philos);
void	take_forks(t_philo *philos);
void	put_the_forks_down(t_philo	*philos);
void	one_philo(t_philo *philos);

//monitor
void	*monitor_routine(void *arg);
int		check_philo_death(t_philo *philo);

int		simulation_should_stop(t_philo *philo);
int		check_and_finish_meals(t_philo *philo);
#endif