#pragma once

#include "Core.h"

class CTimer {
public:
    CTimer ();
    
    void Start ();
    void Stop ();
    void Pause ();
    void Unpause ();
    
    int GetTicks () const;
    
    bool IsStarted () const;
    bool IsPaused () const;

    bool IsWithinTime (real32 seconds) const;

    void SetTicks (uint32 ticks);

    uint32 GetMinutes () const;
    uint32 GetSeconds () const;
    uint32 GetMilliseconds () const;

    void Print (char* string) const;

private:
    int m_startTime;
    int m_pausedTime;

    bool m_paused;
    bool m_started;
};