#include "../include/game/invader.h"
#include <cmath>

Invader::Invader(int startX, int startY) : Entity(startX, startY, 2) {}

void Invader::move(int dx, int dy) {
    Entity::move(dx, dy);
}

void Invader::chase(Entity* target) {
    if (!target) return;

    int dx = 0, dy = 0;
    if (target->getX() > x_) dx = 1;
    else if (target->getX() < x_) dx = -1;
    if (target->getY() > y_) dy = 1;
    else if (target->getY() < y_) dy = -1;

    move(dx, dy);
}

void Invader::attack(Entity* target) {
    if (!target) return;

    int distX = std::abs(x_ - target->getX());
    int distY = std::abs(y_ - target->getY());

    if (distX <= 1 && distY <= 1) {
    }
}