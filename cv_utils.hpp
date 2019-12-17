#ifndef CV_UTILS_HPP
#define CV_UTILS_HPP

#include <chrono>

#include <pthread.h>

////////////////////////////////////////////////////////////////////////
bool initialise_cv(pthread_cond_t *pCv);

////////////////////////////////////////////////////////////////////////
bool wait_for_cv(pthread_cond_t *pCv, pthread_mutex_t *pMutex, const std::chrono::milliseconds &ms);

////////////////////////////////////////////////////////////////////////
bool wait_for_cv(pthread_cond_t *pCv, pthread_mutex_t *pMutex, const std::chrono::milliseconds &ms);

#endif //CV_UTILS_HPP
