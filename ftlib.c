/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtumanya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 19:09:49 by mtumanya          #+#    #+#             */
/*   Updated: 2025/02/12 19:25:11 by mtumanya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

long	ft_atoi(const char *str)
{
	int	sign;
	long	result;

	sign = 1;
	result = 0;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
        if (*str++ == '-')
		    return 0;
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
        if (result > INT_MAX)
			break ;
		str++;
	}
	if (*str != '\0')
        return 0;
	return (sign * result);
}

void*	ft_malloc(size_t size,void *mal)
{
	void *ret;
	ret = malloc(size);
	if (ret == NULL)
	{
		if(mal)
			free(mal);
		printf("Malloc error\n");
		return NULL;
	}
	return ret;
}

void struct_free(t_table *t,int mtx_sim,int mtx_w,int mtx_fork)
{
    if (!t)
	{
        return;
	}
	if(mtx_sim)
		pthread_mutex_destroy(&t->table_lock);
	if(mtx_w)
		pthread_mutex_destroy(&t->write_lock);
    if (t->forks)
    {
		int i = -1;
        while (++i < mtx_fork)
            pthread_mutex_destroy(&t->forks[i].fork);
        free(t->forks);
    }
    if (t->philos)
        free(t->philos);
}

