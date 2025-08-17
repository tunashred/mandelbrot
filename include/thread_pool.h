#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <pthread.h>

typedef void(*thread_func_t)(void* args);

struct tpool_work_t {
    thread_func_t func;
    void* args;
    struct tpool_work_t* next;
} typedef tpool_work_t;

typedef struct {
    tpool_work_t* work_first;
    tpool_work_t* work_last;
    pthread_mutex_t work_mutex;
    pthread_cond_t work_cond;
    pthread_cond_t working_cond;
    size_t working_count;
    size_t thread_count;
    bool stop;
} tpool_t;

tpool_work_t* tpool_work_create(thread_func_t func, void* args);

bool tpool_work_add(tpool_t* tpool, thread_func_t func, void* args);

tpool_work_t* tpool_work_pull(tpool_t* tpool);

void tpool_work_destroy(tpool_work_t* work);

void* tpool_worker(void* args);

tpool_t* tpool_init(size_t thread_count);

void tpool_wait(tpool_t* tpool);

void tpool_destroy(tpool_t* tpool);

#endif
