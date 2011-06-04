#pragma once

#include "Core/Core.h"

class CHash {
public:
    static void Initialize ();
    static uint32 Hash (void* data, uint32 length);
};

