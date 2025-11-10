#pragma once
#include <chrono>

class TimerUtil {
public:
    using clock = std::chrono::steady_clock;
    using ms = std::chrono::milliseconds;

    TimerUtil();

    // Returns true if at least msElapsed milliseconds have passed since last reset/check
    bool hasElapsedMs(int msElapsed) const;

    // Reset the internal timer to "now"
    void reset();

    // Returns milliseconds elapsed since last reset
    long long elapsedMs() const;

private:
    clock::time_point last;
};
