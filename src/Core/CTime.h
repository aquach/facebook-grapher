#pragma once

#include "Core/Core.h"

class CTime {
public:
    CTime (uint32 ticks) : m_ticks(ticks) { }

    void SetTicks (uint32 ticks) {
        m_ticks = ticks;
    }

    uint32 GetMinutes () const {
        return m_ticks / 60000;
    }

    uint32 GetSeconds () const {
        return (m_ticks % 60000) / 1000;
    }

    uint32 GetMilliseconds () const {
        return (m_ticks % 1000) / 100;
    }
    
    void Print (char* string) const {
        sprintf(string, "%.2lu:%.2lu:%.2lu", GetMinutes(), GetSeconds(), GetMilliseconds());
    }

private:
    uint32 m_ticks;
};