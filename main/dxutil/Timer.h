#pragma once

#include "stdafx.h"

namespace dxultra
{

struct Timer
{
    Timer();

    void Start();
    std::chrono::milliseconds Total();
    void Pause();
    void Resume();

  private:
    std::chrono::steady_clock::time_point m_startTimePoint;
    std::chrono::steady_clock::time_point m_lastPausedTimePoint;
    bool m_isPaused;
    std::chrono::nanoseconds m_totalPausedTime;
};

} // namespace dxultra
