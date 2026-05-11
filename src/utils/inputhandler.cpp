#include "../include/utils/inputhandler.h"

Command InputHandler::getCommand(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) return Command::MOVE_DOWN;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) return Command::MOVE_UP;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) return Command::MOVE_LEFT;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) return Command::MOVE_RIGHT;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) return Command::QUIT;

    return Command::NONE;
}

void InputHandler::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
}

void InputHandler::resetLastCommand() {
}