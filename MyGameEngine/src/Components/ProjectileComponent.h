#ifndef PROJECTILECOMPONENT_H
#define PROJECTILECOMPONENT_H

#include <SDL.h>

struct ProjectileComponent
{
    //glm::vec2 projectileVelocity;
    bool isFriendly;
    int hitPercentDamage;
    int duration;
    int startTime;

    struct ProjectileComponent(bool isFriendly = false, int hitPercentDamage = 0, int duration = 0)
    {
        this->isFriendly = isFriendly;
        this->hitPercentDamage = hitPercentDamage;
        this->duration = duration;
        this->startTime = SDL_GetTicks();
    }
};

#endif