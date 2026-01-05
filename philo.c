#include "philos.h"

void destroy_and_free(t_table *t, int philos_inited)
{
    if (!t)
        return ;
    if (t->philos && philos_inited > 0)
    {
        int i = -1;
        while (++i < philos_inited)
            pthread_mutex_destroy(&t->philos[i].p_mtx);
    }
    if (t->forks)
    {
        int i = -1;
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

int main(int argc, char **argv)
{
    t_table table;
    
    if(valid_arg(argc, argv))
        return 1;
    if (init_table(&table, argv))
        return 1;
    if(threads_start(&table))
        return 1;
    destroy_and_free(&table, table.philo_nbr);
    return 0;
}
