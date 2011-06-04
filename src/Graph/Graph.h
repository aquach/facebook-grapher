#pragma once

#include "Core/Core.h"

enum EGender {
    e_genderUnknown,
    e_genderMale,
    e_genderFemale,
};

class CGrapher;

struct SNode;
struct SEdge;

struct SNode {
    string fbId;
    vector<SNode*> neighbors;
    vector<SEdge*> edges;
    string personName;
    EGender gender;

    // Physics
    C3Vector position;
    C3Vector velocity;

    void Render ();
    
    static CGrapher* grapher;
};

struct SEdge;
struct SEdge {
    SNode* one;
    SNode* two;

    SNode* start; // Starter node (one or two) for directionalness
    float percent; // 0 - 100 for whatever you like
    bool animating;

    SEdge () : start(NULL), percent(0.0f), animating(false) { }

    void Render ();
    void Update (uint32 elapsedTicks);

    SNode* GetOtherNode (SNode* node) {
        if (node == one) return two;
        if (node == two) return one;
        return NULL;
    }
};