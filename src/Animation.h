#ifndef ANIMATION_H
#define ANIMATION_H

#include <string>
#include <chrono>
#include <memory>

#include <SDL2/SDL_image.h>


class Animation
{
private:
    unsigned m_currentFrame = 0;
    unsigned m_frameCount = 1;
    // for walking, running... the duration has to change based on speed
    unsigned m_frameDuration = 100;
    unsigned m_sheetRow = 2;
    unsigned m_tileWidth = 64;
    unsigned m_tileHeight = 64;
    std::string m_sourceFile = "";
    std::string m_animationName = "standing";
    uint64_t m_frameStart = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch()).count();

public:
    Animation();

    bool setAnimation(std::string name);
    std::shared_ptr<SDL_Rect> getFrameRect();
};

#endif // ANIMATION_H
