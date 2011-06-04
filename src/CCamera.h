#pragma once

#include "Core/Core.h"

struct SDL_KeyboardEvent;
struct SDL_MouseMotionEvent;

struct CCamera {
public:
    CCamera ();

    real32 GetCameraAngle ();
    void ApplyViewTransform ();

    void SetPosition (C3Vector pos) { position = pos; }
    void SetLookAt (C3Vector look) {lookAt = look; }

    void Move (C3Vector pos) { position += pos; lookAt += pos; }

    void HandleKeyboard (SDL_KeyboardEvent key);
    void HandleMouseMove (SDL_MouseMotionEvent mouse);

    void Update (uint32 elapsedTicks);

    void RotateView (float angle, float x, float y, float z);

private:
    C3Vector position;
    C3Vector lookAt;
    C3Vector up;

    int moveUp;
    int moveRight;
    int moveIn;
    bool speedOn;
    int mouseLastX;
    int mouseLastY;
};
