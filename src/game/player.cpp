#include "../../include/game/player.h"

Player::Player(int startX, int startY) : Entity(startX, startY, 1) {}

void Player::move(int dx, int dy) {
    Entity::move(dx, dy);
}

void Player::updateMovement() {
   
}

void Player::handleInput() {
    
}