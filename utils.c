/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtumanya <mtumanya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 19:44:53 by mtumanya          #+#    #+#             */
/*   Updated: 2026/01/05 20:03:20 by mtumanya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

long	ft_timestamp(t_time_unit unit)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	if (unit == MILISECOND)
		return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
	else
		return (tv.tv_sec * 1000000 + tv.tv_usec);
	return (32132414);
}

void	ft_usleep(long usec, t_table *t)
{
	long	start;
	long	passed;
	long	left;

	start = ft_timestamp(MICROSECOND);
	while (ft_timestamp(MICROSECOND) - start < usec)
	{
		if (get_bool(&t->table_lock, &t->sim_end))
			break ;
		passed = ft_timestamp(MICROSECOND) - start;
		left = usec - passed;
		if (left > 1000)
			usleep(left / 2);
		else
		{
			while (ft_timestamp(MICROSECOND) - start < usec)
				;
		}
	}
}

bool	philo_died(t_philo *p)
{
	long	passed;
	long	to_die;

	if (get_bool(&p->p_mtx, &p->all))
		return (false);
	passed = ft_timestamp(MILISECOND) - get_long(&p->p_mtx, &p->last_time_meal);
	to_die = p->table->time_to_die / 1000;
	if (passed > to_die)
	{
		return (true);
	}
	return (false);
}

void	fair_func(t_philo *p)
{
	if (p->table->philo_nbr % 2 == 0)
	{
		if (p->id % 2 == 0)
			ft_usleep(3e4, p->table);
	}
	else
	{
		if (p->id % 2)
			thinking(p, true);
	}
}

void	*p_routine(void *arg)
{
	t_philo	*p;

	p = (t_philo *)arg;
	wait_treads(p->table);
	set_long(&p->p_mtx, &p->last_time_meal, ft_timestamp(MILISECOND));
	inc(&p->table->table_lock, &p->table->thrun_nmb);
	fair_func(p);
	while (!get_bool(&p->table->table_lock, &p->table->sim_end))
	{
		if (p->all)
			break ;
		eat(p);
		write_state(SLEEPING, p);
		ft_usleep(p->table->time_to_sleep, p->table);
		thinking(p, false);
	}
	return (NULL);
}
