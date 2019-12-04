#include <stdio.h>

#include <iostream>
#include <cassert>
#include <string>

#include <SDL2/SDL.h>

#include "Renderer.h"
#include "Actor.h"
#include "Animation.h"



Actor g_actor;
Animation g_animation;
AssetManager g_assetMgr;



int main()
{
    Renderer renderer;
    g_assetMgr.loadAssets(renderer.getSdlRenderer());

    const Uint8 *keystate = SDL_GetKeyboardState(nullptr);
    if (keystate == nullptr) {
        std::cout << "Failed to get keystate" << std::endl;
        return -1;
    }
    // Boolean data type not supported
    int gameRunning = 1;
    SDL_Event event;

    while (gameRunning) {
        renderer.renderScene();

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                gameRunning = 0;
                break;
            }
        }

        SDL_PumpEvents();
        Vector2D directionVector(0.0, 0.0);
        if (keystate[SDL_SCANCODE_LEFT]  || keystate[SDL_SCANCODE_A]) {
            directionVector.x -= 1.0;
        }
        if (keystate[SDL_SCANCODE_RIGHT] || keystate[SDL_SCANCODE_D]) {
            directionVector.x += 1.0;
        }
        if (keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_W]) {
            directionVector.y -= 1.0;
        }
        if (keystate[SDL_SCANCODE_DOWN] || keystate[SDL_SCANCODE_S]) {
            directionVector.y += 1.0;
        }

        Direction direction = getDirectionFromVector(directionVector);
        g_actor.Move(direction);
        if (direction == Direction::NONE) {
            g_animation.setAnimation("standing");
        }
        else if (direction == Direction::UP) {
            g_animation.setAnimation("walkingUp");
        }
        else if (direction == Direction::DOWN) {
            g_animation.setAnimation("walkingDown");
        }
        else if (direction == Direction::LEFT ||
                 direction == Direction::LEFT_UP ||
                 direction == Direction::LEFT_DOWN) {
            g_animation.setAnimation("walkingLeft");
        }
        else if (direction == Direction::RIGHT ||
                 direction == Direction::RIGHT_UP ||
                 direction == Direction::RIGHT_DOWN) {
            g_animation.setAnimation("walkingRight");
        }

        // TO DO - Replace static value with a time remaining to next frame
        SDL_Delay(10);

    }

    return 0;
}
