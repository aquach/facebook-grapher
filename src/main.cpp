#include "Core/Core.h"
#include "Graph/CGrapher.h"
#include "Gfx/GfxBase.h"
#include <iostream>

CGrapher* grapher = NULL;

//void InitializeGraphics() {
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluPerspective(c_fov, c_aspectRatio, c_nearPlane, c_farPlane);
//
//    glMatrixMode(GL_MODELVIEW);
//}

int main (int argc, char** argv) {
    freopen("error.log", "w", stderr);

    cout << "Enter a graph filename: ";

    char filename[1024];
    cin.getline(filename, 1024);

    // Initialize screen buffer
    SDL_Surface *screen;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    if (!(screen = SDL_SetVideoMode(c_windowWidth, c_windowHeight, 0, SDL_SWSURFACE | SDL_OPENGL))) {
        fprintf(stderr, "Unable to set video mode: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_WM_SetCaption(c_windowTitle, NULL);

    CFont::Initialize();

    grapher = new CGrapher;

    if (strlen(filename) == 0)
        grapher->LoadFile("graph_alex_quach.txt");
    else
        grapher->LoadFile(filename);


    // Begin render loop
    bool quit = false;
    SDL_Event event;
    CTimer frameTimer;
    frameTimer.Start();

    SDL_WM_GrabInput(SDL_GRAB_ON);
    SDL_ShowCursor(SDL_DISABLE);

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    grapher->HandleKeyboard(event.key);
                    if (event.key.keysym.sym == SDLK_PRINT && event.key.type == SDL_KEYDOWN) {
                        Screenshot(screen, "screen.bmp");
                    }
                break;
                case SDL_MOUSEBUTTONUP:
                case SDL_MOUSEBUTTONDOWN:
                    grapher->HandleMouseButton(event.button);
                break;
                case SDL_MOUSEMOTION:
                    grapher->HandleMouseMove(event.motion);
                break;
                case SDL_QUIT:
                    quit = true;
                break;
            }
        }
        if (!frameTimer.IsWithinTime(c_secondsPerFrame)) {
            uint32 elapsedTicks = frameTimer.GetTicks();
            frameTimer.Start();
            grapher->Update(elapsedTicks);
            grapher->Render();
        }
    }

    SDL_Quit();

    return 0;
}
