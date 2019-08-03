#include "Animation.h"
#include "Actor.h"


extern Actor g_actor;


Animation::Animation()
{

}



bool Animation::setAnimation(std::string name)
{
    if (name == m_animationName) {
        return true;
    }

    if (name == "standing" && m_animationName == "walkingUp") {
        m_frameCount = 1;
        m_sheetRow = 0;
    }
    else if (name == "standing" && m_animationName == "walkingLeft") {
        m_frameCount = 1;
        m_sheetRow = 1;
    }
    else if (name == "standing" && m_animationName == "walkingDown") {
        m_frameCount = 1;
        m_sheetRow = 2;
    }
    else if (name == "standing" && m_animationName == "walkingRight") {
        m_frameCount = 1;
        m_sheetRow = 3;
    }
    else if (name == "walkingUp") {
        m_frameCount = 8;
        m_sheetRow = 0;
    }
    else if (name == "walkingLeft") {
        m_frameCount = 8;
        m_sheetRow = 1;
    }
    else if (name == "walkingDown") {
        m_frameCount = 8;
        m_sheetRow = 2;
    }
    else if (name == "walkingRight") {
        m_frameCount = 8;
        m_sheetRow = 3;
    }

    m_animationName = name;
    m_currentFrame = 0;
    m_frameStart = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();

    return true;
}



std::shared_ptr<SDL_Rect> Animation::getFrameRect()
{
    uint64_t currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch()).count();

    if (currentTime - m_frameStart >= m_frameDuration) {
        m_frameStart = currentTime;

        if (m_currentFrame == m_frameCount - 1) {
            m_currentFrame = 0;
        }
        else {
            ++m_currentFrame;
        }
    }

    std::shared_ptr<SDL_Rect> rect = std::make_shared<SDL_Rect>();
    rect->h = m_tileHeight;
    rect->w = m_tileWidth;

    if (m_animationName == "standing") {
        rect->x = m_currentFrame;
    }
    else {
        rect->x = (m_currentFrame + 1) * 64; // first frame in spritesheet is standing
    }
    rect->y = m_sheetRow * 64;

    return rect;
}
