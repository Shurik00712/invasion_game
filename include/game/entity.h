#pragma once

class Entity {
public:
    Entity(int startX = 0, int startY = 0, int speed = 1)
        : x_(startX), y_(startY), speed_(speed) {
    }

    virtual ~Entity() = default;
    virtual void move(int dx, int dy);

    int getX() const { return x_; }
    int getY() const { return y_; }
    int getSpeed() const { return speed_; }
    void setPosition(int x, int y);

protected:
    int x_;
    int y_;
    int speed_;
};