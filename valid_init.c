/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtumanya <mtumanya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 19:44:07 by mtumanya          #+#    #+#             */
/*   Updated: 2026/01/05 20:09:05 by mtumanya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

int	valid_arg(int argc, char **argv)
{
	long	num;
	int		i;

	if (argc != 5 && argc != 6)
		return (printf("wrong arguments count\n"), 1);
	i = 0;
	while (++i < argc)
	{
		num = ft_atoi(argv[i]);
		if (i == 1 && (num < 1 || num > PHILO_MAX))
			return (printf("invalid number_of_philosophers argument\n"), 1);
		if (i == 5 && (num < 1 || num > INT_MAX))
			return (printf("invalid nbr_of_times_philosopher_eat argument\n"),
				1);
		if (i != 1 && i != 5 && num < MS_MIN)
			return (printf("invalid input in 2 3 or 4\n"), 1);
	}
	return (0);
}

int	init_table_2(t_table *t, int i)
{
	if (pthread_mutex_init(&t->philos[i].p_mtx, NULL))
		return (1);
	t->forks[i].id = i;
	t->philos[i].id = i + 1;
	t->philos[i].all = false;
	t->philos[i].meal_num = 0;
	t->philos[i].last_time_meal = 0;
	if (t->philos[i].id % 2 == 0)
	{
		t->philos[i].l_fork = &t->forks[i];
		t->philos[i].r_fork = &t->forks[(i + 1) % t->philo_nbr];
	}
	else
	{
		t->philos[i].l_fork = &t->forks[(i + 1) % t->philo_nbr];
		t->philos[i].r_fork = &t->forks[i];
	}
	t->philos[i].table = t;
	return (0);
}

int	init_2(t_table *t)
{
	if (pthread_mutex_init(&t->table_lock, NULL))
	{
		destroy_and_free(t, 0);
		printf("Mutex init error\n");
		return (1);
	}
	if (pthread_mutex_init(&t->write_lock, NULL))
	{
		destroy_and_free(t, 0);
		printf("Mutex init error\n");
		return (1);
	}
	return (0);
}

int	init(t_table *t, int i)
{
	if (init_2(t))
		return (1);
	while (++i < t->philo_nbr)
	{
		if (pthread_mutex_init(&t->forks[i].fork, NULL))
		{
			printf("Mutex init error\n");
			destroy_and_free(t, i);
			return (1);
		}
	}
	i = -1;
	while (++i < t->philo_nbr)
	{
		if (init_table_2(t, i))
		{
			printf("Mutex init error\n");
			destroy_and_free(t, i);
			return (1);
		}
	}
	return (0);
}

int	init_table(t_table *t, char **argv)
{
	int	i;

	i = -1;
	t->philo_nbr = ft_atoi(argv[1]);
	t->time_to_die = ft_atoi(argv[2]) * 1000;
	t->time_to_eat = ft_atoi(argv[3]) * 1000;
	t->time_to_sleep = ft_atoi(argv[4]) * 1000;
	if (argv[5])
		t->nbr_meals = ft_atoi(argv[5]);
	else
		t->nbr_meals = -1;
	t->thrun_nmb = 0;
	t->sim_end = false;
	t->all_ready = false;
	t->forks = ft_malloc(sizeof(t_fork) * t->philo_nbr, NULL);
	t->philos = ft_malloc(sizeof(t_philo) * t->philo_nbr, t->forks);
	if (t->philos == NULL || t->forks == NULL)
		return (1);
	if (init(t, i))
		return (1);
	return (0);
}
