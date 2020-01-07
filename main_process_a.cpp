
#include <iostream>
#include <memory>
#include <thread>

#include <boost/scope_exit.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>

#include "defs.hpp"
#include "cv_utils.hpp"
#include "mutex_utils.hpp"

////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
    std::cout << "Started Process A" << std::endl;

    namespace bi = boost::interprocess;

    std::unique_ptr<bi::managed_shared_memory> apManagedShMem;
    try
    {
        apManagedShMem = std::make_unique<bi::managed_shared_memory>(bi::open_or_create,
                                                                     MEMORY_KEY,
                                                                     sizeof(pthread_mutex_t) +
                                                                     sizeof(pthread_cond_t) +
                                                                     1024); // Give it a bit of space.
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: Could not create shared memory - " << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "Error: Could not create shared memory - unknown error." << std::endl;
        return 1;
    }

    // NOTE: There was a bug here discovered on SO: Only initialize MUTEX once even if item already exists.
    // TODO: Make this atomic betweeen processes.
    pthread_mutex_t *pMutex = apManagedShMem->find<pthread_mutex_t>("mutex").first;
    if (pMutex == nullptr)
    {
        pMutex = apManagedShMem->find_or_construct<pthread_mutex_t>("mutex")();
        if (pMutex != nullptr && !initialise_mutex(pMutex)) { return 1; }
    }

    if (pMutex == nullptr)
    {
        std::cerr << "Error: Could not instantiate shared mutex." << std::endl;
        return 1;
    }

    // NOTE: There was a bug here discovered on SO: Only initialize CV once even if item already exists.
    // TODO: Make this atomic betweeen processes.
    pthread_cond_t *pCv = apManagedShMem->find<pthread_cond_t>("cv").first;
    if (pCv == nullptr)
    {
        pCv = apManagedShMem->find_or_construct<pthread_cond_t>("cv")();
        if (pCv != nullptr && !initialise_cv(pCv)) { return 1; }
    }

    if (pCv == nullptr)
    {
        std::cerr << "Error: Could not instantiate shared condition variable." << std::endl;
        return 1;
    }

    std::cout << "Commencing wait loop" << std::endl;

    while (true)
    {
        if (!timed_lock_mutex(pMutex, std::chrono::seconds(5)))
        {
            std::cout << "Warning: Mutex wait timeout." << std::endl;
            continue;
        }

        BOOST_SCOPE_EXIT(pMutex)
        {
            unlock_mutex(pMutex);
        } BOOST_SCOPE_EXIT_END

        if (!wait_for_cv(pCv, pMutex, std::chrono::seconds(10)))
        {
            std::cout << "Warning: Wait timeout!" << std::endl;
            continue;
        }

        std::cout << "Info: Received notification." << std::endl;
    }

    return 0;
}

