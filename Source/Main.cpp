
#include <iostream>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include "Application.h"

int main(int argc, char *argv[])
{
    SDL_SetMainReady();
    if(SDL_Init(SDL_INIT_EVENTS) != 0)
    {
        std::cout << SDL_GetError() << std::endl;
    }
    Era::Application::Get().SetArgs({static_cast<size_t>(argc),argv});
    return Era::Application::Get().Start();
}
