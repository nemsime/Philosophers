/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtumanya <mtumanya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 19:45:34 by mtumanya          #+#    #+#             */
/*   Updated: 2026/01/05 20:07:24 by mtumanya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

# define PHILO_MAX 200
# define MS_MIN 60

typedef pthread_mutex_t	t_mtx;
typedef struct table	t_table;

typedef enum e_state
{
	THINKING,
	EATING,
	SLEEPING,
	LEFT_FORK,
	RIGHT_FORK,
	DIED
}						t_state;

typedef enum e_time_unit
{
	MILISECOND,
	MICROSECOND
}						t_time_unit;

typedef struct fork
{
	t_mtx				fork;
	int					id;
}						t_fork;

typedef struct philo
{
	int					id;
	long				meal_num;
	bool				all;
	long				last_time_meal;
	t_fork				*l_fork;
	t_fork				*r_fork;
	pthread_t			thread;
	t_table				*table;
	t_mtx				p_mtx;
}						t_philo;

typedef struct table
{
	long				philo_nbr;
	long				time_to_die;
	long				time_to_eat;
	long				time_to_sleep;
	long				sim_start;
	long				nbr_meals;
	bool				sim_end;
	bool				all_ready;
	t_fork				*forks;
	t_philo				*philos;
	t_mtx				table_lock;
	t_mtx				write_lock;
	pthread_t			monitor;
	long				thrun_nmb;
}						t_table;

void					set_bool(t_mtx *mutex, bool *flag, bool value);
void					set_long(t_mtx *mutex, long *num, long value);
bool					get_bool(t_mtx *mutex, bool *flag);
long					get_long(t_mtx *mutex, long *num);
void					wait_treads(t_table *t);
long					ft_atoi(const char *str);
void					*ft_malloc(size_t size, void *mal);
void					struct_free(t_table *t, int mtx_sim, int mtx_w,
							int mtx_fork);
void					inc(t_mtx *mtx, long *value);
bool					run_threads(t_mtx *mtx, long *t, long p_nbr);
void					destroy_and_free(t_table *t, int philos_inited);
void					*monitoring(void *arg);
void					write_state(t_state state, t_philo *p);
void					eat(t_philo *p);
void					thinking(t_philo *p, bool arg);
void					*just_alone(void *arg);
int						threads_start_2(t_table *t, int i);
int						threads_start(t_table *t);
int						valid_arg(int argc, char **argv);
int						init_table_2(t_table *t, int i);
int						init_2(t_table *t);
int						init(t_table *t, int i);
int						init_table(t_table *t, char **argv);
long					ft_timestamp(t_time_unit unit);
void					ft_usleep(long usec, t_table *t);
bool					philo_died(t_philo *p);
void					fair_func(t_philo *p);
void					*p_routine(void *arg);
int						valid_arg(int argc, char **argv);
int						init_table_2(t_table *t, int i);
int						init_2(t_table *t);
int						init(t_table *t, int i);
int						init_table(t_table *t, char **argv);

#endif