#include "../../include/game/entity.h"

void Entity::move(int dx, int dy) {
    x_ += dx * speed_;
    y_ += dy * speed_;
}

void Entity::setPosition(int x, int y) {
    x_ = x;
    y_ = y;
}