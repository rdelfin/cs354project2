#include <iostream>
#include "menger.h"

namespace {
	const int kMinLevel = 0;
	const int kMaxLevel = 4;
};

Menger::Menger(glm::vec3 min, glm::vec3 max)
    : min(min), max(max), dirty_(true)
{

}

Menger::~Menger()
{
}

void
Menger::set_nesting_level(int level)
{
	nesting_level_ = level;
	dirty_ = true;
}

bool
Menger::is_dirty() const
{
	return dirty_;
}

void
Menger::set_clean()
{
	dirty_ = false;
}

// FIXME generate Menger sponge geometry
void
Menger::generate_geometry(std::vector<glm::vec4>& obj_vertices,
			  std::vector<glm::vec4>& vtx_normals,
                          std::vector<glm::uvec3>& obj_faces) const
{
    if(this->nesting_level_ == 0) {
        generate_cube(obj_vertices, vtx_normals, obj_faces, min, max);
        return;
    }

    //std::cout << "Third: (" << third.x << ", " << third.y << ", " << third.z << ")" << std::endl;
    //std::cout << "ThirdX: (" << thirdx.x << ", " << thirdx.y << ", " << thirdx.z << ")" << std::endl;
    //std::cout << "ThirdY: (" << thirdy.x << ", " << thirdy.y << ", " << thirdy.z << ")" << std::endl;
    //std::cout << "ThirdZ: (" << thirdz.x << ", " << thirdz.y << ", " << thirdz.z << ")" << std::endl;

    std::vector<glm::vec3> mins;
    mins.push_back(this->min);

    for(int l = 1; l <= nesting_level_; l++) {
        glm::vec3 third = (max - min) * float(1.0f / pow(3.0f, l));
        glm::vec3 thirdx = glm::vec3(third.x, 0, 0);
        glm::vec3 thirdy = glm::vec3(0, third.y, 0);
        glm::vec3 thirdz = glm::vec3(0, 0, third.z);

        std::vector<glm::vec3> subcubeMins;
        subcubeMins = mins;
        mins.clear();

        for(glm::vec3 minvec : subcubeMins) {
            mins.push_back(minvec);                                                 // Bottom left front
            mins.push_back(minvec + thirdx);                                        // Bottom center front
            mins.push_back(minvec + 2.0f * thirdx);                                 // Bottom right front
            mins.push_back(minvec + thirdz);                                        // Bottom left middle
            mins.push_back(minvec + thirdz + 2.0f * thirdx);                        // Bottom right middle
            mins.push_back(minvec + 2.0f * thirdz);                                 // Bottom left back
            mins.push_back(minvec + 2.0f * thirdz + thirdx);                        // Bottom center back
            mins.push_back(minvec + 2.0f * thirdz + 2.0f * thirdx);                 // Bottom right back

            mins.push_back(minvec + thirdy);                                        // Center left front
            mins.push_back(minvec + thirdy + 2.0f * thirdz);                        // Center left back
            mins.push_back(minvec + thirdy + 2.0f * thirdz + 2.0f * thirdx);        // Center right back
            mins.push_back(minvec + thirdy + 2.0f * thirdx);                        // Center right front

            mins.push_back(minvec + 2.0f * thirdy);                                 // Top left front
            mins.push_back(minvec + 2.0f * thirdy + thirdx);                        // Top center front
            mins.push_back(minvec + 2.0f * thirdy + 2.0f * thirdx);                 // Top right front
            mins.push_back(minvec + 2.0f * thirdy + thirdz);                        // Top left middle
            mins.push_back(minvec + 2.0f * thirdy + thirdz + 2.0f * thirdx);        // Top right middle
            mins.push_back(minvec + 2.0f * thirdy + 2.0f * thirdz);                 // Top left back
            mins.push_back(minvec + 2.0f * thirdy + 2.0f * thirdz + thirdx);        // Top center back
            mins.push_back(minvec + 2.0f * thirdy + 2.0f * thirdz + 2.0f * thirdx); // Top right back
        }
    }

    glm::vec3 cubeDiag = (max - min) * float(1.0 / pow(3.0f, nesting_level_));
    for(auto it = mins.begin(); it != mins.end(); ++it)
        generate_cube(obj_vertices, vtx_normals, obj_faces, *it, *it + cubeDiag);

    std::cout << "Created " << mins.size() << " cubes" << std::endl;
}

void Menger::generate_cube(std::vector<glm::vec4> &obj_vertices, std::vector<glm::vec4> &vtx_normals,
                           std::vector<glm::uvec3> &obj_faces, glm::vec3 min, glm::vec3 max) const {

    unsigned long idx = obj_vertices.size();
    // Front face
    obj_vertices.push_back(glm::vec4(max.x, min.y, min.z, 1.0f));
    vtx_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(max.x, max.y, min.z, 1.0f));
    vtx_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(min.x, max.y, min.z, 1.0f));
    vtx_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(min.x, min.y, min.z, 1.0f));
    vtx_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));

    obj_faces.push_back(glm::uvec3(idx, idx+ 1, idx + 2));
    obj_faces.push_back(glm::uvec3(idx, idx + 2, idx + 3));

    // Back face
    obj_vertices.push_back(glm::vec4(max.x, min.y, max.z, 1.0f));
    vtx_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(max.x, max.y, max.z, 1.0f));
    vtx_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(min.x, max.y, max.z, 1.0f));
    vtx_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(min.x, min.y, max.z, 1.0f));
    vtx_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));

    obj_faces.push_back(glm::uvec3(idx + 4, idx + 5, idx + 6));
    obj_faces.push_back(glm::uvec3(idx + 4, idx + 6, idx + 7));

    // Right face
    obj_vertices.push_back(glm::vec4(max.x, min.y, max.z, 1.0f));
    vtx_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(max.x, max.y, max.z, 1.0f));
    vtx_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(max.x, max.y, min.z, 1.0f));
    vtx_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(max.x, min.y, min.z, 1.0f));
    vtx_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));

    obj_faces.push_back(glm::uvec3(idx + 8, idx + 9, idx + 10));
    obj_faces.push_back(glm::uvec3(idx + 8, idx + 10, idx + 11));


    // Left face
    obj_vertices.push_back(glm::vec4(min.x, min.y, max.z, 1.0f));
    vtx_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(min.x, max.y, max.z, 1.0f));
    vtx_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(min.x, max.y, min.z, 1.0f));
    vtx_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(min.x, min.y, min.z, 1.0f));
    vtx_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));

    obj_faces.push_back(glm::uvec3(idx + 12, idx + 13, idx + 14));
    obj_faces.push_back(glm::uvec3(idx + 12, idx + 14, idx + 15));

    // Top face
    obj_vertices.push_back(glm::vec4(min.x, max.y, max.z, 1.0f));
    vtx_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(min.x, max.y, min.z, 1.0f));
    vtx_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(max.x, max.y, min.z, 1.0f));
    vtx_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(max.x, max.y, max.z, 1.0f));
    vtx_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));

    obj_faces.push_back(glm::uvec3(idx + 16, idx + 17, idx + 18));
    obj_faces.push_back(glm::uvec3(idx + 16, idx + 18, idx + 19));

    // Bottom face
    obj_vertices.push_back(glm::vec4(min.x, min.y, max.z, 1.0f));
    vtx_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(min.x, min.y, min.z, 1.0f));
    vtx_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(max.x, min.y, min.z, 1.0f));
    vtx_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(max.x, min.y, max.z, 1.0f));
    vtx_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));

    obj_faces.push_back(glm::uvec3(idx + 20, idx + 21, idx + 22));
    obj_faces.push_back(glm::uvec3(idx + 20, idx + 22, idx + 23));
}



