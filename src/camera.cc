#include <iostream>
#include <sstream>
#include "camera.h"

#include <glm/gtx/rotate_vector.hpp>

namespace {
	float pan_speed = 0.1f;
	float roll_speed = 0.1f;
	float rotation_speed = 0.05f;
	float zoom_speed = 0.1f;
};

void Camera::rotate(glm::vec3 dir) {
    eye_ = glm::rotateY(eye_, rotation_speed*dir.x);
    eye_ = glm::rotateX(eye_, rotation_speed*dir.y);
    eye_ = glm::rotateZ(eye_, rotation_speed*dir.z);
}


// FIXME: Calculate the view matrix
glm::mat4 Camera::get_view_matrix() const
{
    glm::vec3 Z = glm::normalize(eye_ - look_);
    glm::vec3 X = glm::cross(up_, Z);
    glm::vec3 Y = glm::normalize(glm::cross(Z, X));
    X = glm::normalize(X);

    // Yes the first three could have been in a loop but SHUSH!
    glm::mat4 eyeMat(1.0f);
    eyeMat[0] = glm::vec4(X.x, Y.x, Z.x, 0);
    eyeMat[1] = glm::vec4(X.y, Y.y, Z.y, 0);
    eyeMat[2] = glm::vec4(X.z, Y.z, Z.z, 0);
    eyeMat[3] = glm::vec4(glm::dot(-X, eye_), glm::dot(-Y, eye_), glm::dot(-Z, eye_), 1);

    return eyeMat;
}

