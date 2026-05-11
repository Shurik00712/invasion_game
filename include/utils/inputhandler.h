#pragma once
#include "../include/game/game_logic.h"
#include <GLFW/glfw3.h>

class InputHandler {
public:
    static Command getCommand(GLFWwindow* window);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void resetLastCommand();
};