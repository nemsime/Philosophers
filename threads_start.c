/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_start.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtumanya <mtumanya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 19:43:39 by mtumanya          #+#    #+#             */
/*   Updated: 2026/01/05 20:00:55 by mtumanya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

void	eat(t_philo *p)
{
	pthread_mutex_lock(&p->l_fork->fork);
	write_state(LEFT_FORK, p);
	pthread_mutex_lock(&p->r_fork->fork);
	write_state(RIGHT_FORK, p);
	set_long(&p->p_mtx, &p->last_time_meal, ft_timestamp(MILISECOND));
	p->meal_num++;
	write_state(EATING, p);
	ft_usleep(p->table->time_to_eat, p->table);
	if (p->table->nbr_meals > 0 && p->meal_num == p->table->nbr_meals)
		set_bool(&p->p_mtx, &p->all, true);
	pthread_mutex_unlock(&p->l_fork->fork);
	pthread_mutex_unlock(&p->r_fork->fork);
}

void	thinking(t_philo *p, bool arg)
{
	long	to_eat;
	long	to_sleep;
	long	to_think;

	if (!arg)
		write_state(THINKING, p);
	if (p->table->philo_nbr % 2 == 0)
		return ;
	to_eat = p->table->time_to_eat;
	to_sleep = p->table->time_to_sleep;
	to_think = to_eat * 2 - to_sleep;
	if (to_think < 0)
		to_think = 0;
	ft_usleep(to_think * 0.42, p->table);
}

void	*just_alone(void *arg)
{
	t_philo	*p;

	p = (t_philo *)arg;
	wait_treads(p->table);
	set_long(&p->p_mtx, &p->last_time_meal, ft_timestamp(MILISECOND));
	inc(&p->table->table_lock, &p->table->thrun_nmb);
	write_state(LEFT_FORK, p);
	while (!get_bool(&p->table->table_lock, &p->table->sim_end))
		usleep(200);
	return (NULL);
}

int	threads_start_2(t_table *t, int i)
{
	if (t->philo_nbr == 1)
	{
		if (pthread_create(&t->philos[0].thread, NULL, just_alone,
				&t->philos[0]))
			return (1);
	}
	else
	{
		while (++i < t->philo_nbr)
			if (pthread_create(&t->philos[i].thread, NULL, p_routine,
					&t->philos[i]))
				return (1);
	}
	return (0);
}

int	threads_start(t_table *t)
{
	int	i;

	i = -1;
	if (threads_start_2(t, i))
		return (1);
	if (pthread_create(&t->monitor, NULL, monitoring, t))
		return (1);
	t->sim_start = ft_timestamp(MILISECOND);
	set_bool(&t->table_lock, &t->all_ready, true);
	i = -1;
	while (++i < t->philo_nbr)
		if (pthread_join(t->philos[i].thread, NULL))
			return (1);
	set_bool(&t->table_lock, &t->sim_end, true);
	if (pthread_join(t->monitor, NULL))
		return (1);
	return (0);
}
