//
// Created by rdelfin on 10/11/16.
//

#include "input.hpp"

MouseState::MouseState(GLFWwindow*) {

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


KeyState::KeyState(GLFWwindow* window) {

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


void KeyState::update() {
    for(unsigned i = 0; i < N_KEY_BUTTONS; i++)
        prev[i] = curr[i];
}

KeyState::~KeyState() {

}
