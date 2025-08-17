#include "thread_pool.h"

tpool_work_t* tpool_work_create(thread_func_t func, void* args) {
    if(!func || !args) {
        return NULL;
    }

    tpool_work_t* work = (tpool_work_t*) malloc(sizeof *work);
    work->func = func;
    work->args = args;
    work->next = NULL;

    return work;
}

bool tpool_work_add(tpool_t* tpool, thread_func_t func, void* args) {
    if(!tpool) {
        return false;
    }

    tpool_work_t* work = tpool_work_create(func, args);
    if(!work) {
        return false;
    }

    pthread_mutex_lock(&tpool->work_mutex);
    if(!tpool->work_first) {
        tpool->work_first = work;
        tpool->work_last = work;
    } else {
        tpool->work_last->next = work;
        tpool->work_last = work;
    }
    pthread_cond_broadcast(&tpool->work_cond);
    pthread_mutex_unlock(&tpool->work_mutex);

    return true;
}

tpool_work_t* tpool_work_pull(tpool_t* tpool) {
    if(!tpool) {
        return NULL;
    }

    tpool_work_t* work = tpool->work_first;
    if(!work) {
        return NULL;
    }

    if(!work->next) {
        tpool->work_first = NULL;
        tpool->work_last = NULL;
    }
    else {
        tpool->work_first = work->next;
    }

    return work;
}

void tpool_work_destroy(tpool_work_t* work) {
    if(!work) {
        return;
    }
    free(work);
}

void* tpool_worker(void* args) {
    tpool_t* tpool = (tpool_t*) args;
    tpool_work_t* work = NULL;
    while(1) {
        pthread_mutex_lock(&tpool->work_mutex);
        
        while(!tpool->work_first && !tpool->stop) {
            pthread_cond_wait(&tpool->work_cond, &tpool->work_mutex);
        }
        if(tpool->stop) {
            break;
        }

        work = tpool_work_pull(tpool);
        tpool->working_count++;
        pthread_mutex_unlock(&tpool->work_mutex);

        if(work) {
            work->func(work->args);
            tpool_work_destroy(work);
        }

        pthread_mutex_lock(&tpool->work_mutex);
        tpool->working_count--;
        if(!tpool->stop && !tpool->work_first && tpool->working_count == 0) {
            pthread_cond_signal(&tpool->working_cond);
        }
        pthread_mutex_unlock(&tpool->work_mutex);
    }
    tpool->thread_count--;
    pthread_cond_signal(&tpool->working_cond);
    pthread_mutex_unlock(&tpool->work_mutex);

    return NULL;
}

tpool_t* tpool_init(size_t thread_count) {
    if(thread_count <= 0) {
        thread_count = 2;
        printf("Unexpected thread count argument given. Using default setting for %ld threads", thread_count);
    }

    tpool_t* tpool = (tpool_t*) calloc(1, sizeof *tpool);
    tpool->thread_count = thread_count;
    pthread_mutex_init(&tpool->work_mutex, NULL);
    pthread_cond_init(&tpool->work_cond, NULL);
    pthread_cond_init(&tpool->working_cond, NULL);

    tpool->work_first = NULL;
    tpool->work_last = NULL;

    pthread_t thread;
    for(size_t i = 0; i < thread_count; i++) {
        pthread_create(&thread, NULL, tpool_worker, tpool);
        pthread_detach(thread);
    }
    return tpool;
}

void tpool_wait(tpool_t* tpool) {
    if(!tpool) {
        return;
    }
    
    pthread_mutex_lock(&tpool->work_mutex);
    while(1) {
        if(tpool->work_first ||
        (tpool->thread_count != 0 && tpool->stop) ||
        (tpool->working_count != 0 && !tpool->stop)) {
            pthread_cond_wait(&tpool->working_cond, &tpool->work_mutex);
        }
        else {
            break;
        }
    }
    pthread_mutex_unlock(&tpool->work_mutex);
}

void tpool_destroy(tpool_t* tpool) {
    if(!tpool) {
        return;
    }

    tpool_work_t* current_work;
    tpool_work_t* next_work;

    pthread_mutex_lock(&tpool->work_mutex);
    current_work = tpool->work_first;
    while(current_work) {
        next_work = current_work->next;
        tpool_work_destroy(current_work);
        current_work = next_work;
    }
    tpool->work_first = NULL;
    tpool->stop = true;
    pthread_cond_broadcast(&tpool->work_cond);
    pthread_mutex_unlock(&tpool->work_mutex);

    tpool_wait(tpool);

    pthread_mutex_destroy(&tpool->work_mutex);
    pthread_cond_destroy(&tpool->work_cond);
    pthread_cond_destroy(&tpool->working_cond);

    free(tpool);
    tpool = NULL;
}
