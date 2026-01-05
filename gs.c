/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtumanya <mtumanya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 19:41:57 by mtumanya          #+#    #+#             */
/*   Updated: 2026/01/05 19:57:52 by mtumanya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

void	set_bool(t_mtx *mutex, bool *flag, bool value)
{
	pthread_mutex_lock(mutex);
	*flag = value;
	pthread_mutex_unlock(mutex);
}
void	set_long(t_mtx *mutex, long *num, long value)
{
	pthread_mutex_lock(mutex);
	*num = value;
	pthread_mutex_unlock(mutex);
}
bool	get_bool(t_mtx *mutex, bool *flag)
{
	bool	value;

	pthread_mutex_lock(mutex);
	value = *flag;
	pthread_mutex_unlock(mutex);
	return (value);
}

long	get_long(t_mtx *mutex, long *num)
{
	long	value;

	pthread_mutex_lock(mutex);
	value = *num;
	pthread_mutex_unlock(mutex);
	return (value);
}

void	wait_treads(t_table *t)
{
	while (!get_bool(&t->table_lock, &t->all_ready))
		;
}
