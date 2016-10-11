#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera {
public:
	glm::mat4 get_view_matrix() const;

    void rotateX(float dir);
    void rotateY(float dir);
    void translate(glm::vec3 dir);
	void zoom(float dir);
private:
	float camera_distance_ = 3.0;
	glm::vec3 look_ = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 center_ = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 up_ = glm::vec3(0.0f, 1.0, 0.0f);
	glm::vec3 eye_ = glm::vec3(0.0f, 0.0f, camera_distance_);

    glm::mat4 translateMat = glm::mat4(1.0f);
    glm::mat4 rotateMat = glm::mat4(1.0f);
};

#endif
