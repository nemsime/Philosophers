#include "philos.h"

static void eat(t_philo *p)
{
    pthread_mutex_lock(&p->l_fork->fork);
    write_state(LEFT_FORK,p);
    pthread_mutex_lock(&p->r_fork->fork);
    write_state(RIGHT_FORK,p);

    set_long(&p->p_mtx,&p->last_time_meal,ft_timestamp(MILISECOND));
    p->meal_num++;
    write_state(EATING,p);
    ft_usleep(p->table->time_to_eat,p->table);
    if(p->table->nbr_meals > 0 && p->meal_num == p->table->nbr_meals)
        set_bool(&p->p_mtx,&p->all, true);
    pthread_mutex_unlock(&p->l_fork->fork);
    pthread_mutex_unlock(&p->r_fork->fork);
}

void thinking(t_philo *p,bool arg)
{
    long to_eat;
    long to_sleep;
    long to_think;

    if(!arg)
        write_state(THINKING, p);
    if(p->table->philo_nbr % 2 == 0)
        return;
    to_eat = p->table->time_to_eat;
    to_sleep = p->table->time_to_sleep;
    to_think = to_eat * 2 - to_sleep;
    if(to_think < 0)
        to_think = 0;
    ft_usleep(to_think * 0.42,p->table);
}

void    write_state(t_state state,t_philo *p)
{
    long passed;

    passed = ft_timestamp(MILISECOND) - p->table->sim_start;
    if(p->all)
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

long    ft_timestamp(t_time_unit unit)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    if (unit == MILISECOND)
        return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
    else
        return (tv.tv_sec * 1000000 + tv.tv_usec);
    return (32132414);
}

void    ft_usleep(long usec,t_table *t)
{
    long start;
    long passed;
    long left;

    start = ft_timestamp(MICROSECOND);
    while (ft_timestamp(MICROSECOND) - start < usec)
    {
        if(get_bool(&t->table_lock,&t->sim_end))
            break;
        passed = ft_timestamp(MICROSECOND) - start;
        left = usec - passed;
        if(left > 1000)
            usleep(left / 2);
        else
        {
            while(ft_timestamp(MICROSECOND) - start < usec)
                ;
        }
    }
}

 
void wait_treads(t_table *t)
{
    while(!get_bool(&t->table_lock,&t->all_ready))
        ;
}
void    fair_func(t_philo *p)
{
    if(p->table->philo_nbr % 2 == 0)
    {
        if(p->id % 2 == 0)
            ft_usleep(3e4,p->table);
    }
    else
    {
        if(p->id % 2)
            thinking(p, true);
    }
}

void *p_routine(void *arg)
{
    t_philo *p = (t_philo *)arg;
    wait_treads(p->table);

    set_long(&p->p_mtx,&p->last_time_meal,ft_timestamp(MILISECOND));

    inc(&p->table->table_lock,&p->table->thrun_nmb);

    fair_func(p);
    while(!get_bool(&p->table->table_lock,&p->table->sim_end))
    {
        if (p->all)
            break;
        eat(p);

        write_state(SLEEPING, p);
        ft_usleep(p->table->time_to_sleep, p->table);

        thinking(p, false);
    }
    return NULL;
}
static bool philo_died(t_philo *p)
{
    long passed;
    long to_die;
    if(get_bool(&p->p_mtx,&p->all))
        return (false);

    passed = ft_timestamp(MILISECOND) - get_long(&p->p_mtx,&p->last_time_meal);
    to_die = p->table->time_to_die / 1000;
    if(passed > to_die)
    {
        return (true);
    }
    return (false);
}

void    *monitoring(void *arg)
{
    t_table *t;
    int i;

    t = (t_table *)arg;

    while(!run_threads(&t->table_lock,&t->thrun_nmb,t->philo_nbr))
        ;
    while(!get_bool(&t->table_lock,&t->sim_end))
    {
        i = -1;
        while(++i < t->philo_nbr)
        {
            if(philo_died(&t->philos[i]))
            {
                write_state(DIED,&t->philos[i]);                
                set_bool(&t->table_lock,&t->sim_end, true);
            }
        }

    }
    return NULL;
}

void *just_alone(void *arg)
{
    t_philo *p = (t_philo *)arg;
    wait_treads(p->table);

    set_long(&p->p_mtx,&p->last_time_meal,ft_timestamp(MILISECOND));

    inc(&p->table->table_lock,&p->table->thrun_nmb);
    write_state(LEFT_FORK,p);
    while(!get_bool(&p->table->table_lock,&p->table->sim_end))
        usleep(200);
    return NULL; 
}

int threads_start(t_table *t)
{
    int i;
    i = -1;
    if(t->philo_nbr == 1)
    {
        if(pthread_create(&t->philos[0].thread, NULL, just_alone, &t->philos[0]))
                return 1;
    }
    else
    {
        while(++i < t->philo_nbr)
            if(pthread_create(&t->philos[i].thread, NULL, p_routine, &t->philos[i]))
                return 1;
    }
    if(pthread_create(&t->monitor, NULL, monitoring, t))
                return 1;
    t->sim_start = ft_timestamp(MILISECOND);
    set_bool(&t->table_lock, &t->all_ready,true);
    i = -1;
    while(++i < t->philo_nbr)
        if(pthread_join(t->philos[i].thread, NULL))
            return 1;
    set_bool(&t->table_lock,&t->sim_end,true);
    if(pthread_join(t->monitor, NULL))
            return 1;
    return 0;
}
