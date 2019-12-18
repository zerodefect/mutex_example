#include "mutex_utils.hpp"

#include <cassert>
#include <iostream>

#include "clock_wait.hpp"

///////////////////////////////////////////////////////////////////////////
class mutexattr_releaser final
{
public:

    mutexattr_releaser() = delete;

    explicit mutexattr_releaser(pthread_mutexattr_t &mutexattr) :
        m_mutexattr(mutexattr)
    {

    }

    mutexattr_releaser(const mutexattr_releaser &m) = delete;
    mutexattr_releaser(mutexattr_releaser &&m) = delete;

    mutexattr_releaser &operator=(const mutexattr_releaser &) = delete;
    mutexattr_releaser &operator=(mutexattr_releaser &&) = delete;

    ~mutexattr_releaser()
    {
        pthread_mutexattr_destroy(&m_mutexattr);
    }

private:
    pthread_mutexattr_t &m_mutexattr;
};

////////////////////////////////////////////////////////////////////////
bool initialise_mutex(pthread_mutex_t *const pMutex)
{
    pthread_mutexattr_t mutexattr{};
    if (0 != pthread_mutexattr_init(&mutexattr))
    {
        std::cerr << "Error: Failed to initialise shared mutex attributes.";
        return false;
    }

    // Need to cleanup mutexattr once we are finished with it.
    const mutexattr_releaser releaser(mutexattr);

    if (0 != pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_NORMAL))
    {
        std::cerr << "Error: Failed to initialise shared mutex type to NORMAL." << std::endl;
        return false;
    }

    if (0 != pthread_mutexattr_setpshared(&mutexattr, PTHREAD_PROCESS_SHARED))
    {
        std::cerr << "Error: Failed to set shared mutex attributes to shared" << std::endl;
        return false;
    }

    if (0 != pthread_mutexattr_setrobust(&mutexattr, PTHREAD_MUTEX_ROBUST))
    {
        std::cerr << "Error: Failed to set shared mutex to robust." << std::endl;
        return false;
    }

    if (0 != pthread_mutex_init(pMutex, &mutexattr))
    {
        std::cerr << "Error: Failed to initialise shared mutex." << std::endl;
        return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////
void lock_mutex(pthread_mutex_t *pMutex)
{
    const int n = pthread_mutex_lock(pMutex);
    if (0 == n)
    {

    }
    else if (EOWNERDEAD == n)
    {
        pthread_mutex_consistent(pMutex);
    }
    else
    {
        assert(n == 0);
    }
}

////////////////////////////////////////////////////////////////////////
bool try_lock_mutex(pthread_mutex_t *pMutex)
{
    const int n = pthread_mutex_trylock(pMutex);
    if (0 == n)
    {
        return true;
    }
    else if (EOWNERDEAD == n)
    {
        pthread_mutex_consistent(pMutex);
        return true;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////
void unlock_mutex(pthread_mutex_t *pMutex)
{
    const int n = pthread_mutex_unlock(pMutex);
    assert(n == 0);
}

///////////////////////////////////////////////////////////////////////////
bool timed_lock(pthread_mutex_t *const pMutex, const timespec *time)
{
    const int n = pthread_mutex_timedlock(pMutex, time);
    if (0 == n)
    {
        return true;
    }
    else if (ETIMEDOUT == n)
    {
        return false;
    }
    else if (EOWNERDEAD == n)
    {
        pthread_mutex_consistent(pMutex);
        return true;
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////
bool timed_lock_mutex(pthread_mutex_t *pMutex, const std::chrono::milliseconds &ms)
{
    timespec tp{};
    if (!get_current_time_offset(tp, ms)) { return false; }

    return timed_lock(pMutex, &tp);
}

///////////////////////////////////////////////////////////////////////////
bool timed_lock_mutex(pthread_mutex_t *pMutex, const std::chrono::seconds &s)
{
    timespec tp{};
    if (!get_current_time_offset(tp, s)) { return false; }

    return timed_lock(pMutex, &tp);
}
