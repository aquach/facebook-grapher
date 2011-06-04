#include "CTimer.h"

#include "SDL/SDL.h"
#include <cstdlib>

CTimer::CTimer ()
{
    m_startTime = 0;
    m_pausedTime = 0;
    m_paused = false;
    m_started = false;    
}

void CTimer::Start ()
{
    m_started = true;
    m_paused = false;
    m_startTime = SDL_GetTicks();    
}

void CTimer::Stop ()
{
    m_started = false;
    m_paused = false;    
}

//-------------------------------------------------------------------------------------------------------
void CTimer::Pause () {
    if (m_started == true && m_paused == false) {
        m_paused = true;
        m_pausedTime = SDL_GetTicks() - m_startTime;
    }
}

//-------------------------------------------------------------------------------------------------------
void CTimer::Unpause () {
    if (m_paused == true) {
        m_paused = false;
        m_startTime = SDL_GetTicks() - m_pausedTime;
        m_pausedTime = 0;
    }
}

//-------------------------------------------------------------------------------------------------------
int CTimer::GetTicks () const {
    if (m_started == true) {
        if (m_paused == true)
            return m_pausedTime;
        else
            return SDL_GetTicks() - m_startTime;
    }
    
    return 0;    
}

//-------------------------------------------------------------------------------------------------------
bool CTimer::IsStarted () const {
    return m_started;    
}

//-------------------------------------------------------------------------------------------------------
bool CTimer::IsPaused () const {
    return m_paused;    
}

//-------------------------------------------------------------------------------------------------------
bool CTimer::IsWithinTime (real32 seconds) const {
    return IsStarted() && (GetTicks() < seconds * 1000);
}

//-------------------------------------------------------------------------------------------------------
void CTimer::SetTicks (uint32 ticks) {
    m_startTime = SDL_GetTicks() - ticks;
}

//-------------------------------------------------------------------------------------------------------
uint32 CTimer::GetMinutes () const {    
    return CTime(GetTicks()).GetMinutes();
}

//-------------------------------------------------------------------------------------------------------
uint32 CTimer::GetSeconds () const {
    return CTime(GetTicks()).GetSeconds();
}

//-------------------------------------------------------------------------------------------------------
uint32 CTimer::GetMilliseconds () const {
    return CTime(GetTicks()).GetMilliseconds();
}

//-------------------------------------------------------------------------------------------------------
void CTimer::Print (char* string) const {
    CTime time(GetTicks());
    time.Print(string);
}