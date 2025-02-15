#include "Timer.h"

namespace dxultra
{

Timer::Timer()
    : m_startTimePoint{std::chrono::high_resolution_clock::now()}, m_isPaused{false},
      m_totalPausedTime{0}
{
}

void Timer::Start()
{
    m_startTimePoint = std::chrono::high_resolution_clock::now();
}

std::chrono::milliseconds Timer::Total()
{
    auto now = std::chrono::high_resolution_clock::now();
    auto elapsed = now - m_startTimePoint - m_totalPausedTime;

    return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
}

void Timer::Pause()
{
    if (m_isPaused)
    {
        return;
    }

    m_lastPausedTimePoint = std::chrono::high_resolution_clock::now();
    m_isPaused = true;
}

void Timer::Resume()
{
    if (!m_isPaused)
    {
        return;
    }
    auto now = std::chrono::high_resolution_clock::now();
    auto elapsedPauseTime = now - m_lastPausedTimePoint;
    m_totalPausedTime += elapsedPauseTime;
    m_isPaused = false;
}

} // namespace dxultra