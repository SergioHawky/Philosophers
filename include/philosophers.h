/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seilkiv <seilkiv@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 16:22:47 by seilkiv           #+#    #+#             */
/*   Updated: 2025/11/25 16:22:47 by seilkiv          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

# define PHILOS_MAX 200

typedef enum e_status
{
	THINKING,
	TAKING_FORK,
	EATING,
	SLEEPING,
	DEAD
}					t_status;

typedef struct s_vars
{
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_times_each_philosopher_must_eat;
	volatile int	start_flag;
	pthread_mutex_t	*mutexes;
	pthread_mutex_t	print_mutex;
	size_t			start_time;
	int				shutdown_flag;
	pthread_mutex_t	shutdown_mutex;
}					t_vars;

typedef struct s_philos
{
	int				id;
	pthread_t		thread;
	int				left_fork;
	int				right_fork;
	size_t			time_last_eaten;
	pthread_mutex_t	time_last_eaten_mutex;
	int				death_flag;
	int				times_eaten;
	pthread_mutex_t	times_eaten_mutex;
	t_vars			*vars;
}					t_philos;

typedef struct s_master
{
	pthread_t		master_thread;
	t_vars			*vars;
	t_philos		**philosophers;
	int				total_meals;

}					t_master;

// init.c
t_philos			**init_philos(t_vars *vars);
void				init_threads(t_vars *vars, t_philos **philos_array,
						t_master *master);
void				init_mutexes(t_philos **philos_array, t_vars *vars);
void				create_forks(t_philos **philos_array, t_vars *vars);

// utils.c
size_t				get_time(void);
void				print(t_philos *thread, char *msg);
void				free_philosophers(t_vars *vars, t_philos **philos_array);
void				destroy_mutexes(t_vars *vars, t_philos **philos);
int					ft_atoi(const char *str);

// monitor.c
void				*monitor(void *arg);

// routine.c
int					check_for_death(t_philos *thread);
void				go_to_sleep(t_philos *thread);
void				take_forks(t_philos *thread);
void				eat(t_philos *thread);
void				*routine(void *arg);

#endif