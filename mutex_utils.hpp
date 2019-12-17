#ifndef MUTEX_UTILS_HPP
#define MUTEX_UTILS_HPP

#include <chrono>

#include <pthread.h>

////////////////////////////////////////////////////////////////////////
bool initialise_mutex(pthread_mutex_t *pMutex);

////////////////////////////////////////////////////////////////////////
void lock_mutex(pthread_mutex_t *pMutex);

////////////////////////////////////////////////////////////////////////
bool try_lock_mutex(pthread_mutex_t *pMutex);

////////////////////////////////////////////////////////////////////////
void unlock_mutex(pthread_mutex_t *pMutex);

////////////////////////////////////////////////////////////////////////
bool timed_lock_mutex(pthread_mutex_t *pMutex, const std::chrono::milliseconds &ms);

////////////////////////////////////////////////////////////////////////
bool timed_lock_mutex(pthread_mutex_t *pMutex, const std::chrono::seconds &s);

#endif //MUTEX_UTILS_HPP