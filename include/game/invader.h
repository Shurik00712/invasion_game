#pragma once
#include "entity.h"

class Invader : public Entity {
public:
    Invader(int startX = 0, int startY = 0);
    void move(int dx, int dy) override;
    void chase(Entity* target);
    void attack(Entity* target);
};