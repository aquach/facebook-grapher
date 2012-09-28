#include "CTexture.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "Resources/ResourceConstants.h"
#include <iostream>
using namespace std;

CTextureTable             CTexture::s_textures;

#define SET_RED(P, C)   (P = (((P) & 0xffffff00) | (C)))
#define SET_GREEN(P, C) (P = (((P) & 0xffff00ff) | ((C) << 8)))
#define SET_BLUE(P, C)  (P = (((P) & 0xff00ffff) | ((C) << 16)))

inline void LoadTex (string filename, uint32*& data, uint32* width, uint32* height){

    filename = string(c_textureDirectory) + filename;
    SDL_Surface* tex;
    tex = IMG_Load(filename.c_str());
    if (tex == NULL)
        return;

    int w = tex->w;
    int h = tex->h;

    *width = w;
    *height = h;

    SDL_Surface* newSurface;
    bool hasAlpha = (tex->format->BytesPerPixel == 4);
    cout << hasAlpha << endl;
    if (hasAlpha && false) {
        newSurface = SDL_CreateRGBSurface(0, w, h, 32,
                                          0x000000ff,
                                          0x0000ff00,
                                          0x00ff0000,
                                          0xff000000);
        SDL_SetAlpha(tex, 0, 0);
    } else {
        newSurface = SDL_CreateRGBSurface(0, w, h, 24,
                                          0xff000000,
                                          0x0000ff00,
                                          0x00ff0000,
                                          0);
    }
    SDL_BlitSurface(tex, 0, newSurface, 0);

    data = (uint32*)newSurface->pixels;
}

//-------------------------------------------------------------------------------------------------------
CTexture* CTexture::LoadTexture (const char* name, const char* filename) {
    uint32* data = NULL;
    uint32 height, width;

    LoadTex(filename, data, &width, &height);

    if (data == NULL) {
        // Fix: send error or something
        return NULL;
    }

    GLuint glTexture;
    glGenTextures(1, &glTexture);
    glBindTexture(GL_TEXTURE_2D, glTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    CTexture* tex = new CTexture;
    tex->m_glTexture = glTexture;
    tex->m_name = name;
    tex->m_width = width;
    tex->m_height = height;

    cout << glGetError() << endl;

    delete[] data;

    return tex;
}

//-------------------------------------------------------------------------------------------------------
CTexture* CTexture::Get (const char* name) {
    CTextureTable::iterator iter = s_textures.find(name);
    if (iter == s_textures.end())
        return NULL;

    return iter->second;
}
