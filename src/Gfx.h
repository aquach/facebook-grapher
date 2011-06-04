#pragma once

#include "GL/glew.h"
#include "GL/gl.h"
#include "GL/glu.h"

#include "SDL/SDL.h"
#define NO_SDL_GLEXT
#include "SDL/SDL_opengl.h"

const char* const c_windowTitle = "Facebook Graph";
const real32 c_framesPerSecond = 30;
const real32 c_secondsPerFrame = 1.0f / c_framesPerSecond;
const uint32 c_windowWidth = 1024;
const uint32 c_windowHeight = 768;
const real32 c_aspectRatio = (real32)c_windowWidth / c_windowHeight;
const real32 c_nearPlane = 5.0f;
const real32 c_farPlane = 500.0f;
const real32 c_fov = 45.0f;