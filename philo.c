/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtumanya <mtumanya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 19:42:40 by mtumanya          #+#    #+#             */
/*   Updated: 2026/01/05 20:00:02 by mtumanya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

void	destroy_and_free(t_table *t, int philos_inited)
{
	int	i;

	if (!t)
		return ;
	if (t->philos && philos_inited > 0)
	{
		i = -1;
		while (++i < philos_inited)
			pthread_mutex_destroy(&t->philos[i].p_mtx);
	}
	if (t->forks)
	{
		i = -1;
		while (++i < t->philo_nbr)
			pthread_mutex_destroy(&t->forks[i].fork);
		free(t->forks);
		t->forks = NULL;
	}
	pthread_mutex_destroy(&t->table_lock);
	pthread_mutex_destroy(&t->write_lock);
	if (t->philos)
	{
		free(t->philos);
		t->philos = NULL;
	}
}

void	*monitoring(void *arg)
{
	t_table	*t;
	int		i;

	t = (t_table *)arg;
	while (!run_threads(&t->table_lock, &t->thrun_nmb, t->philo_nbr))
		;
	while (!get_bool(&t->table_lock, &t->sim_end))
	{
		i = -1;
		while (++i < t->philo_nbr)
		{
			if (philo_died(&t->philos[i]))
			{
				write_state(DIED, &t->philos[i]);
				set_bool(&t->table_lock, &t->sim_end, true);
			}
		}
	}
	return (NULL);
}

void	write_state(t_state state, t_philo *p)
{
	long	passed;

	passed = ft_timestamp(MILISECOND) - p->table->sim_start;
	if (p->all)
		return ;
	pthread_mutex_lock(&p->table->write_lock);
	if (!get_bool(&p->table->table_lock, &p->table->sim_end))
	{
		if (state == LEFT_FORK || state == RIGHT_FORK)
			printf("%-6ld %d has taken a fork\n", passed, p->id);
		else if (state == EATING)
			printf("%-6ld %d is eating\n", passed, p->id);
		else if (state == SLEEPING)
			printf("%-6ld %d is sleeping\n", passed, p->id);
		else if (state == THINKING)
			printf("%-6ld %d is thinking\n", passed, p->id);
		else if (state == DIED)
			printf("%-6ld %d died\n", passed, p->id);
	}
	pthread_mutex_unlock(&p->table->write_lock);
}

int	main(int argc, char **argv)
{
	t_table	table;

	if (valid_arg(argc, argv))
		return (1);
	if (init_table(&table, argv))
		return (1);
	if (threads_start(&table))
		return (1);
	destroy_and_free(&table, table.philo_nbr);
	return (0);
}
