#include "TimerUtil.h"

TimerUtil::TimerUtil() : last(clock::now()) {}

bool TimerUtil::hasElapsedMs(int msElapsed) const {
    auto now = clock::now();
    auto diff = std::chrono::duration_cast<ms>(now - last).count();
    return diff >= msElapsed;
}

void TimerUtil::reset() {
    last = clock::now();
}

long long TimerUtil::elapsedMs() const {
    auto now = clock::now();
    return std::chrono::duration_cast<ms>(now - last).count();
}
