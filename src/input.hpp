//
// Created by rdelfin on 10/11/16.
//

#ifndef GLSL_INPUT_HPP
#define GLSL_INPUT_HPP

class MouseState {
public:
    MouseState(GLFWwindow*);
    bool pressed(int button);
    bool released(int button);
    bool justPressed(int button);
    bool justReleased(int button);

    glm::vec2 pos();
    glm::vec2 delPos();

    ~MouseState();
private:
    bool curr[8];   // GLFW defines up to 8 mouse buttons
    bool prev[8];
    glm::vec4 pos;
    glm::vec4 prevPos;
};

class KeyState {
public:
    KeyState(GLFWwindow*);
    bool pressed(int button);
    bool released(int button);
    bool justPressed(int button);
    bool justReleased(int button);
    ~KeyState();
private:
    bool curr[GLFW_KEY_LAST + 1];
    bool prev[GLFW_KEY_LAST + 1];

};


#endif //GLSL_INPUT_HPP
