//
// Created by rdelfin on 10/11/16.
//

#include "input.hpp"
#include <boost/bind.hpp>

MouseState::MouseState(GLFWwindow* window) {
    glfwSetCursorPosCallback(window, boost::bind(&MouseState::MousePosCallback, this));
    glfwSetMouseButtonCallback(window, boost::bind(&MouseState::MouseButtonCallback, this));
}

bool MouseState::pressed(unsigned button) {
    currMutex.lock();
    bool result = button >= N_MOUSE_BUTTONS ? false : curr[button];
    currMutex.unlock();
    return result;
}

bool MouseState::released(unsigned button) {
    currMutex.lock();
    bool result = button >= N_MOUSE_BUTTONS ? false : !curr[button];
    currMutex.unlock();
    return result;
}

bool MouseState::justPressed(unsigned button) {
    currMutex.lock();
    bool result =  button >= N_MOUSE_BUTTONS ? false : curr[button] && !prev[button];
    currMutex.unlock();
    return result;
}

bool MouseState::justReleased(unsigned button) {
    currMutex.lock();
    bool result = button >= N_MOUSE_BUTTONS ? false : !curr[button] && prev[button];
    currMutex.unlock();
    return result;
}

glm::vec2 MouseState::getPos() {
    posMutex.lock();
    glm::vec2 res = pos;
    posMutex.unlock();
    return res;
}

glm::vec2 MouseState::delPos() {
    posMutex.lock();
    glm::vec2 res = pos - prevPos;
    posMutex.unlock();
    return res;
}

MouseState::~MouseState() {

}

void MouseState::update() {
    currMutex.lock();
    pos = prevPos;
    for(unsigned i = 0; i < N_MOUSE_BUTTONS; i++)
        prev[i] = curr[i];
    currMutex.unlock();
}

void MouseState::MousePosCallback(GLFWwindow *window, double mouseX, double mouseY) {
    posMutex.lock();
    pos = glm::vec2(mouseX, mouseY);
    posMutex.unlock();
}

void MouseState::MouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    currMutex.lock();
    if(action == GLFW_PRESS || action == GLFW_RELEASE)
        curr[button] = action == GLFW_PRESS;
    currMutex.unlock();
}




KeyState::KeyState(GLFWwindow* window) {
    glfwSetKeyCallback(window, std::bind(&KeyState::KeyCallback, this));
}

bool KeyState::pressed(unsigned button) {
    currMutex.lock();
    bool result = button >= N_KEY_BUTTONS ? false : curr[button];
    currMutex.unlock();
    return result;
}

bool KeyState::released(unsigned button) {
    currMutex.lock();
    bool result = button >= N_KEY_BUTTONS ? false : !curr[button];
    currMutex.unlock();
    return result;
}

bool KeyState::justPressed(unsigned button) {
    currMutex.lock();
    bool result =  button >= N_KEY_BUTTONS ? false : curr[button] && !prev[button];
    currMutex.unlock();
    return result;
}

bool KeyState::justReleased(unsigned button) {
    currMutex.lock();
    bool result = button >= N_KEY_BUTTONS ? false : !curr[button] && prev[button];
    currMutex.unlock();
    return result;
}

void KeyState::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    currMutex.lock();
    if(action == GLFW_PRESS || action == GLFW_RELEASE)
        curr[key] = action == GLFW_PRESS;
    currMutex.unlock();
}

void KeyState::update() {
    currMutex.lock();
    for(unsigned i = 0; i < N_KEY_BUTTONS; i++)
        prev[i] = curr[i];
    currMutex.unlock();
}

KeyState::~KeyState() {

}