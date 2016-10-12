//
// Created by rdelfin on 10/11/16.
//

#include <mutex>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#ifndef GLSL_INPUT_HPP
#define GLSL_INPUT_HPP

#define N_MOUSE_BUTTONS 8
#define N_KEY_BUTTONS (GLFW_KEY_LAST + 1)

class MouseState {
public:
    MouseState(GLFWwindow*);
    bool pressed(unsigned button);
    bool released(unsigned button);
    bool justPressed(unsigned button);
    bool justReleased(unsigned button);

    glm::vec2 getPos();
    glm::vec2 delPos();

    void update();

    ~MouseState();
private:
    bool curr[N_MOUSE_BUTTONS];   // GLFW defines up to 8 mouse buttons
    bool prev[N_MOUSE_BUTTONS];
    glm::vec2 pos;
    glm::vec2 prevPos;

    std::mutex currMutex;
    std::mutex posMutex;

    void MousePosCallback(GLFWwindow* window, double mouseX, double mouseY);
    void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
};

class KeyState {
public:
    KeyState(GLFWwindow*);

    bool pressed(unsigned button);
    bool released(unsigned button);
    bool justPressed(unsigned button);
    bool justReleased(unsigned button);

    void update();

    ~KeyState();
private:
    bool curr[N_KEY_BUTTONS];
    bool prev[N_KEY_BUTTONS];

    void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    std::mutex currMutex;

};


#endif //GLSL_INPUT_HPP
