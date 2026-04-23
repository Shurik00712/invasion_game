// input_handler.cpp
#include "../include/utils/inputhandler.h"

Command InputHandler::lastCommand_ = Command::NONE;

Command InputHandler::getCommand(GLFWwindow* window) {
    Command cmd = lastCommand_;
    lastCommand_ = Command::NONE;
    return cmd;
}

void InputHandler::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS) return;

    switch (key) {
    case GLFW_KEY_W: lastCommand_ = Command::MOVE_DOWN; break;
    case GLFW_KEY_S: lastCommand_ = Command::MOVE_UP; break;
    case GLFW_KEY_A: lastCommand_ = Command::MOVE_LEFT; break;
    case GLFW_KEY_D: lastCommand_ = Command::MOVE_RIGHT; break;
    case GLFW_KEY_ESCAPE: lastCommand_ = Command::QUIT; break;
    case GLFW_KEY_SPACE: lastCommand_ = Command::SHOOT; break;
    default: break;
    }
}

void InputHandler::resetLastCommand() {
    lastCommand_ = Command::NONE;
}