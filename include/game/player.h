#pragma once
#include "entity.h"

class Player : public Entity {
public:
    Player(int startX = 0, int startY = 0);
    void move(int dx, int dy) override;
    void updateMovement() override; 
    void handleInput();
};