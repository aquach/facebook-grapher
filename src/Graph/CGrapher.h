#pragma once

#include "Core/Core.h"
#include "Graph.h"
#include "CCamera.h"
#include "Gfx/CFont.h"

struct SDL_MouseButtonEvent;
struct SDL_KeyboardEvent;
struct SDL_MouseMotionEvent;

class CGrapher {
public:
    CGrapher ();
    ~CGrapher () { }

    void LoadFile (const char* filename);

    void Update (uint32 elapsedTicks);
    void Render ();
    void HandleKeyboard (SDL_KeyboardEvent key);
    void HandleMouseButton (SDL_MouseButtonEvent mouseButton);
    void HandleMouseMove (SDL_MouseMotionEvent mouse );

    void DoPhysicsUpdate (uint32 elapsedTicks);

private:
    vector<SNode*> m_nodes;
    vector<SEdge*> m_edges;
    CCamera m_camera;
    CFont* m_font;

    string m_focusPerson;
    bool   m_focusMode;
    real32 m_kineticEnergy;
    bool   m_paused;
    uint32 m_updateTime;
};