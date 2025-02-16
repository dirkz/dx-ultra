#pragma once

#include "stdafx.h"

namespace dxultra
{

struct Timer
{
    Timer();

    void Start();

    /// <summary>
    /// Milliseconds elapsed since <code>Start</code>, not counting pauses.
    /// </summary>
    /// <returns>Milliseconds as <code>std::chrono::milliseconds</code></returns>
    std::chrono::milliseconds ElapsedMillisecounds();

    /// <summary>
    /// Seconds elapsed since <code>Start</code>, not counting pauses.
    /// </summary>
    /// <returns>Seconds as <code>double</code></returns>
    double ElapsedSeconds();

    void Pause();
    void Resume();

  private:
    std::chrono::steady_clock::time_point m_startTimePoint;
    std::chrono::steady_clock::time_point m_lastPausedTimePoint;
    bool m_isPaused;
    std::chrono::nanoseconds m_totalPausedTime;
};

} // namespace dxultra
