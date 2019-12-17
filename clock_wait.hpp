#ifndef CLOCK_WAIT_HPP
#define CLOCK_WAIT_HPP

///////////////////////////////////////////////////////////////////////////
inline bool get_current_time(timespec &ts)
{
    const int i = clock_gettime(CLOCK_REALTIME, &ts);
    if (i == 0) { return true; }

    // Bad if the clock makes it here.
    assert(false);

    ts = {};
    return false;
}

///////////////////////////////////////////////////////////////////////////
inline bool get_current_time_offset(timespec &ts, const std::chrono::nanoseconds &t)
{
    if (!get_current_time(ts)) { return false; }

    const std::chrono::nanoseconds::rep ns = std::chrono::duration_cast<std::chrono::nanoseconds>(t).count();

    ts.tv_sec += (ts.tv_nsec + ns) / 1000000000;
    ts.tv_nsec = (ts.tv_nsec + ns) % 1000000000;

    return true;
}

///////////////////////////////////////////////////////////////////////////
inline bool get_current_time_offset(timespec &ts, const std::chrono::milliseconds &t)
{
    return get_current_time_offset(ts, std::chrono::duration_cast<std::chrono::nanoseconds>(t));
}

///////////////////////////////////////////////////////////////////////////
inline bool get_current_time_offset(timespec &ts, const std::chrono::seconds &t)
{
    if (!get_current_time(ts)) { return false; }

    const std::chrono::seconds::rep s = std::chrono::duration_cast<std::chrono::seconds>(t).count();

    ts.tv_sec += s;
    return true;
}

#endif //CLOCK_WAIT_HPP
