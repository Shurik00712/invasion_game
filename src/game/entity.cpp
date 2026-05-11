#include "../../include/game/entity.h"
#include <cmath>

void Entity::move(int dx, int dy) {
    if (isMoving_) return;

    targetX_ += dx;
    targetY_ += dy;
    isMoving_ = true;
}

void Entity::setPosition(int x, int y) {
    targetX_ = x;
    targetY_ = y;
    visualX_ = (float)x;
    visualY_ = (float)y;
    isMoving_ = false;
}