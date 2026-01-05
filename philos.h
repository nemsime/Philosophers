#ifndef PHILO_H
#define PHILO_H

#include <stdio.h> // printf
#include <stdlib.h> // exit, malloc
#include <limits.h> // INT_MAX
#include <pthread.h> // threads
#include <unistd.h> // sleep
#include <stdbool.h> // bool
#include <sys/time.h>

#define PHILO_MAX 200
#define MS_MIN 60
typedef pthread_mutex_t t_mtx;
typedef struct table t_table;

typedef enum e_state
{
    THINKING,
    EATING,
    SLEEPING,
    LEFT_FORK,
    RIGHT_FORK,
    DIED
}   t_state;


typedef enum e_time_unit
{
    MILISECOND,
    MICROSECOND
}   t_time_unit;

typedef struct fork
{
    t_mtx fork;
    int id;
} t_fork;

typedef struct philo
{
    int id;
    long meal_num;
    bool all;
    long last_time_meal;
    t_fork* l_fork;
    t_fork* r_fork;
    pthread_t thread; 
    t_table *table;
    t_mtx   p_mtx; // useful for races with the monitor
} t_philo;

typedef struct table
{
    long philo_nbr;
    long time_to_die;
    long time_to_eat;
    long time_to_sleep;
    long sim_start;
    long nbr_meals; // optional    
    bool sim_end; // dead or full
    bool all_ready;
    t_fork* forks;
    t_philo* philos;
    t_mtx table_lock;
    t_mtx write_lock;
    pthread_t monitor;
    long    thrun_nmb;
} t_table;

long ft_atoi(const char *str);

int valid_arg(int argc,char **argv);
int init_table(t_table *table,char **argv);
void* ft_malloc(size_t size,void *mal);
void struct_free(t_table *t,int mtx_sim,int mtx_w,int mtx_fork);
int threads_start(t_table *t);
void set_bool(t_mtx *mutex,bool *flag, bool value);
void set_long(t_mtx *mutex,long *num, long value);
bool get_bool(t_mtx *mutex, bool *flag);
long get_long(t_mtx *mutex, long *num);
long    ft_timestamp(t_time_unit unit);
void    ft_usleep(long usec,t_table *t);
void    write_state(t_state state,t_philo *p);
bool run_threads(t_mtx *mtx,long *t,long p_nbr);
void inc(t_mtx *mtx,long *value);
void thinking(t_philo *p, bool arg);
void destroy_and_free(t_table *t, int philos_inited);


#endif