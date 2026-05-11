// player.cpp
#include "../../include/game/player.h"
#include <cmath>

Player::Player(int startX, int startY) : Entity(startX, startY, 0.1f) {}

void Player::move(int dx, int dy) {
    Entity::move(dx, dy);
}

void Player::updateMovement() {
    if (!isMoving_) return;

    float dx = targetX_ - visualX_;
    float dy = targetY_ - visualY_;

    if (std::abs(dx) > 0.001f) {
        visualX_ += (dx > 0 ? 1.0f : -1.0f) * speed_;
        if (std::abs(visualX_ - targetX_) < speed_) {
            visualX_ = (float)targetX_;
        }
    }

    if (std::abs(dy) > 0.001f) {
        visualY_ += (dy > 0 ? 1.0f : -1.0f) * speed_;
        if (std::abs(visualY_ - targetY_) < speed_) {
            visualY_ = (float)targetY_;
        }
    }

    if (std::abs(visualX_ - targetX_) < 0.001f && std::abs(visualY_ - targetY_) < 0.001f) {
        visualX_ = (float)targetX_;
        visualY_ = (float)targetY_;
        isMoving_ = false;
    }
}

void Player::handleInput() {
}