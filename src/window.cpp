#include "window.hpp"
#include <SDL2/SDL.h>

void init_window(Window* window)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    Uint32 flags = SDL_WINDOW_OPENGL;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window->window = SDL_CreateWindow("Made with Rainstorm", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window->width, window->height, flags);
    window->context = SDL_GL_CreateContext(window->window);
    window->running = true;
}

void update_window(Window* window)
{
    SDL_Event ev;
    
    while(SDL_PollEvent(&ev))
    {
        if(ev.type == SDL_QUIT)
        {
            window->running = false;
        }
        if(ev.type == SDL_KEYDOWN)
        {
            if(ev.key.keysym.sym == SDLK_ESCAPE) window->running = false;
        }
    }
}