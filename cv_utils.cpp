
#include "cv_utils.hpp"

#include <cassert>
#include <iostream>

#include "clock_wait.hpp"

///////////////////////////////////////////////////////////////////////////
class condattr_releaser final
{
public:

    condattr_releaser() = delete;

    explicit condattr_releaser(pthread_condattr_t &condattr) :
        m_condattr(condattr)
    {

    }

    condattr_releaser(const condattr_releaser &m) = delete;
    condattr_releaser(condattr_releaser &&m) = delete;

    condattr_releaser &operator=(const condattr_releaser &) = delete;
    condattr_releaser &operator=(condattr_releaser &&) = delete;

    ~condattr_releaser()
    {
        pthread_condattr_destroy(&m_condattr);
    }

private:
    pthread_condattr_t &m_condattr;
};

////////////////////////////////////////////////////////////////////////
bool initialise_cv(pthread_cond_t *const pCv)
{
    pthread_condattr_t condattr{};
    if (0 != pthread_condattr_init(&condattr))
    {
        std::cerr << ("Failed to initialise interprocess_condition attributes.") << std::endl;
        return false;
    }

    const condattr_releaser releaser(condattr);

    if (0 != pthread_condattr_setpshared(&condattr, PTHREAD_PROCESS_SHARED))
    {
        std::cerr << ("Error: Failed to set interprocess_condition attributes to shared.") << std::endl;
        return false;
    }

    if (0 != pthread_condattr_setclock(&condattr, CLOCK_REALTIME))
    {
        std::cerr << ("Error: Failed to set interprocess_condition attributes of clock to REALTIME.") << std::endl;
        return false;
    }

    if (0 != pthread_cond_init(pCv, &condattr))
    {
        std::cerr << ("Error: Failed to initialise interprocess_condition") << std::endl;
        return false;
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////
bool wait_until(pthread_cond_t *pCv, pthread_mutex_t *pMutex, const timespec *time)
{
    const int n = pthread_cond_timedwait(pCv, pMutex, time);
    if (0 == n)
    {
        return true;
    }
    else if (ETIMEDOUT == n)
    {
        return false;
    }

    assert(false);
    return false;
}

///////////////////////////////////////////////////////////////////////////
bool wait_for_cv(pthread_cond_t *pCv, pthread_mutex_t *pMutex, const std::chrono::milliseconds &ms)
{
    timespec tp{};
    if (!get_current_time_offset(tp, ms)) { return false; }

    return wait_until(pCv, pMutex, &tp);
}

///////////////////////////////////////////////////////////////////////////
bool wait_for_cv(pthread_cond_t *pCv, pthread_mutex_t *pMutex, const std::chrono::seconds &s)
{
    timespec tp{};
    if (!get_current_time_offset(tp, s)) { return false; }

    return wait_until(pCv, pMutex, &tp);
}

