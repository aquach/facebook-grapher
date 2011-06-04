#include "CCamera.h"
#include "Gfx/GfxBase.h"
#include <iostream>

const real32 c_cameraSpeed = 7.5f;
const real32 c_cameraSpeedMult = 3.0f;
const real32 c_cameraMouseSensitivity = 0.5f;

//-------------------------------------------------------------------------------------------------------
CCamera::CCamera () :
    position(1.0f, 1.0f, 1.0f),
    lookAt(0.0f, 0.0f, 0.0f),
    up(0.0f, 0.0f, 1.0f),
    moveUp(0),
    moveRight(0),
    moveIn(0),
    speedOn(false),
    mouseLastX(c_windowWidth / 2),
    mouseLastY(c_windowHeight / 2) { }

//-------------------------------------------------------------------------------------------------------
real32 CCamera::GetCameraAngle () {
    C3Vector dir = lookAt - position;
    if (dir.x == 0) dir.x = 0.0001f;
    real32 angle = atan2(dir.y, dir.x) - 3.1415f * 0.5f;
    return angle;
}

//-------------------------------------------------------------------------------------------------------
void CCamera::ApplyViewTransform () {
    gluLookAt(
        position.x, position.y, position.z,
        lookAt.x, lookAt.y, lookAt.z,
        up.x, up.y, up.z
    );
}

//-------------------------------------------------------------------------------------------------------
void CCamera::Update (uint32 elapsedTicks) {
    C3Vector dir = lookAt - position;
    dir.Normalize();
    C3Vector right = C3Vector::Cross(dir, up);
    right.Normalize();
    C3Vector newUp = C3Vector::Cross(right, dir);
    newUp.Normalize();

    real32 dt = (real32)elapsedTicks * 0.001f;

    real32 speedMult = speedOn ? c_cameraSpeedMult : 1.0f;

    Move(newUp * c_cameraSpeed * dt * moveUp * speedMult);
    Move(right * c_cameraSpeed * dt * moveRight * speedMult);
    Move(dir * c_cameraSpeed * dt * moveIn * speedMult);

    // Mouse stuff
    int x, y;
    SDL_GetMouseState(&x, &y);
    SDL_WarpMouse(c_windowWidth / 2, c_windowHeight / 2);
    int dx = x - c_windowWidth / 2;
    int dy = y - c_windowHeight / 2;
    if (dx != 0 || dy != 0) {
        real32 angleY = -(real32)dy * 0.1f * c_cameraMouseSensitivity;
        real32 angleZ = -(real32)dx * 0.1f * c_cameraMouseSensitivity;

        RotateView(angleY, right.x, right.y, right.z);
        RotateView(angleZ, newUp.x, newUp.y, newUp.z);
    }
}

//-------------------------------------------------------------------------------------------------------
void CCamera::HandleKeyboard (SDL_KeyboardEvent key) {

    switch (key.keysym.sym) {
        case SDLK_a:
            if (key.type == SDL_KEYDOWN)
                moveRight = -1;
             else
                moveRight = 0;

        break;
        case SDLK_d:
            if (key.type == SDL_KEYDOWN)
                moveRight = 1;
             else
                moveRight = 0;
        break;
        case SDLK_w:
            if (key.type == SDL_KEYDOWN)
                moveIn = 1;
             else
                moveIn = 0;
        break;
        case SDLK_s:
            if (key.type == SDL_KEYDOWN)
                moveIn = -1;
             else
                moveIn = 0;
        break;
        case SDLK_LCTRL:
            if (key.type == SDL_KEYDOWN)
                speedOn = true;
             else
                speedOn = false;
        break;
        default:
        break;
    }
}

//-------------------------------------------------------------------------------------------------------
void CCamera::HandleMouseMove (SDL_MouseMotionEvent mouse) {
}

//-------------------------------------------------------------------------------------------------------
void CCamera::RotateView (float angle, real32 x, real32 y, real32 z)
{
    C3Vector vNewView;

    // Get the view vector (The direction we are facing)
    C3Vector vView = lookAt - position;

    // Calculate the sine and cosine of the angle once
    real32 cosTheta = (real32)cos(CMath::ToRadians(angle));
    real32 sinTheta = (real32)sin(CMath::ToRadians(angle));

    // Find the new x position for the new rotated point
    vNewView.x  = (cosTheta + (1 - cosTheta) * x * x)        * vView.x;
    vNewView.x += ((1 - cosTheta) * x * y - z * sinTheta)    * vView.y;
    vNewView.x += ((1 - cosTheta) * x * z + y * sinTheta)    * vView.z;

    // Find the new y position for the new rotated point
    vNewView.y  = ((1 - cosTheta) * x * y + z * sinTheta)    * vView.x;
    vNewView.y += (cosTheta + (1 - cosTheta) * y * y)        * vView.y;
    vNewView.y += ((1 - cosTheta) * y * z - x * sinTheta)    * vView.z;

    // Find the new z position for the new rotated point
    vNewView.z  = ((1 - cosTheta) * x * z - y * sinTheta)    * vView.x;
    vNewView.z += ((1 - cosTheta) * y * z + x * sinTheta)    * vView.y;
    vNewView.z += (cosTheta + (1 - cosTheta) * z * z)        * vView.z;

    // Now we just add the newly rotated vector to our position to set
    // our new rotated view of our camera.
    lookAt = position + vNewView;
}
