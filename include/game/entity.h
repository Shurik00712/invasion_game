#pragma once

class Entity {
public:
    Entity(int startX = 0, int startY = 0, float speed = 0.05f)
        : targetX_(startX), targetY_(startY), visualX_((float)startX), visualY_((float)startY),
        speed_(speed), isMoving_(false) {
    }

    virtual ~Entity() = default;
    virtual void move(int dx, int dy);
    virtual void updateMovement() = 0;

    int getX() const { return targetX_; }
    int getY() const { return targetY_; }
    float getVisualX() const { return visualX_; }
    float getVisualY() const { return visualY_; }
    float getSpeed() const { return speed_; }

    void setPosition(int x, int y);
    void setSpeed(float speed) { speed_ = speed; }
    bool isMoving() const { return isMoving_; }

protected:
    int targetX_, targetY_;
    float visualX_, visualY_;
    float speed_;
    bool isMoving_;
};