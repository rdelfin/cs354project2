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



void Camera::rotateX(float dir) {
    rotateMat = glm::rotate(rotation_speed*dir, up_) * rotateMat;

    glm::vec3 newEye(rotateMat * glm::vec4(eye_, 1));
    look_ = glm::normalize(center_ - newEye);
    right_ = glm::normalize(glm::cross(look_, up_));
    up_ = glm::normalize(glm::cross(right_, look_));
}

void Camera::rotateY(float dir) {
    rotateMat = glm::rotate(rotation_speed*dir, right_) * rotateMat;

    glm::vec3 newEye(rotateMat * glm::vec4(eye_, 1));
    look_ = glm::normalize(center_ - newEye);
    right_ = glm::normalize(glm::cross(look_, up_));
    up_ = glm::normalize(glm::cross(right_, look_));
}


void Camera::translate(glm::vec2 dir) {
    translateMat  = glm::translate((dir.x*right_ + dir.y*up_)*pan_speed) * translateMat;
}

void Camera::zoom(float dir) {
    camera_distance_ += dir*zoom_speed;
    eye_ = glm::vec3(0.0f, 0.0f, camera_distance_);
}

glm::mat4 Camera::get_view_matrix() const
{
    glm::vec3 newEye(translateMat * rotateMat * glm::vec4(eye_, 1));
    glm::vec3 newCenter(translateMat * rotateMat * glm::vec4(center_, 1));

    glm::vec3 Z = glm::normalize(newEye - newCenter);
    glm::vec3 X = glm::cross(up_, Z);
    glm::vec3 Y = glm::normalize(glm::cross(Z, X));
    X = glm::normalize(X);

    // Yes the first three could have been in a loop but SHUSH!
    glm::mat4 eyeMat(1.0f);
    eyeMat[0] = glm::vec4(X.x, Y.x, Z.x, 0);
    eyeMat[1] = glm::vec4(X.y, Y.y, Z.y, 0);
    eyeMat[2] = glm::vec4(X.z, Y.z, Z.z, 0);
    eyeMat[3] = glm::vec4(glm::dot(-X, newEye), glm::dot(-Y, newEye), glm::dot(-Z, newEye), 1);

    return eyeMat;
}

