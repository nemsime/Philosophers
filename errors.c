#include "philos.h"

void set_bool(t_mtx *mutex,bool *flag, bool value)
{
    pthread_mutex_lock(mutex);
    *flag = value;
    pthread_mutex_unlock(mutex);
}
void set_long(t_mtx *mutex,long *num, long value)
{
    pthread_mutex_lock(mutex);
    *num = value;
    pthread_mutex_unlock(mutex);
}

bool get_bool(t_mtx *mutex, bool *flag)
{
    bool value;

    pthread_mutex_lock(mutex);
    value = *flag;
    pthread_mutex_unlock(mutex);
    return value;
}

long get_long(t_mtx *mutex, long *num)
{
    long value;

    pthread_mutex_lock(mutex);
    value = *num;
    pthread_mutex_unlock(mutex);
    return value;
}

void inc(t_mtx *mtx,long *value)
{
    pthread_mutex_lock(mtx);
    (*value)++;
    pthread_mutex_unlock(mtx);
}

bool run_threads(t_mtx *mtx,long *t,long p_nbr)
{
    bool ret; 
    ret = false;
    pthread_mutex_lock(mtx);
    if(*t == p_nbr)
        ret = true;
    pthread_mutex_unlock(mtx);
    return (ret);
}

