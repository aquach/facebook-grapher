#pragma once

#include "Core/Core.h"

struct SNode;
struct SNode {
    uint fbId;
    vector<SNode*> neighbors;

    // Physics
    C3Vector position;
    C3Vector velocity;

    void Render ();
};