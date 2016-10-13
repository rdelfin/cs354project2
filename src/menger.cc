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
                          std::vector<glm::uvec3>& obj_faces) const {
    if(nesting_level_ == 0) {
        generate_cube(&obj_vertices, &vtx_normals, &obj_faces);
        return;
    }

    glm::vec3 third = (max - min) / 3.0f;


    glm::vec3 thirdx = glm::vec3(third.x, 0, 0);
    glm::vec3 thirdy = glm::vec3(0, third.y, 0);
    glm::vec3 thirdz = glm::vec3(0, 0, third.z);

    //std::cout << "Third: (" << third.x << ", " << third.y << ", " << third.z << ")" << std::endl;
    //std::cout << "ThirdX: (" << thirdx.x << ", " << thirdx.y << ", " << thirdx.z << ")" << std::endl;
    //std::cout << "ThirdY: (" << thirdy.x << ", " << thirdy.y << ", " << thirdy.z << ")" << std::endl;
    //std::cout << "ThirdZ: (" << thirdz.x << ", " << thirdz.y << ", " << thirdz.z << ")" << std::endl;

    std::vector<Menger> subcubes;

    subcubes.push_back(Menger(min, min + third));                                                                                     // Bottom left front
    subcubes.push_back(Menger(min + thirdx, min + thirdx + third));                                                                   // Bottom center front
    //subcubes.push_back(Menger(min + 2.0f*thirdx, min + 2.0f*thirdx + third));                                                         // Bottom right front
    //subcubes.push_back(Menger(min + thirdz, min + thirdz + third));                                                                   // Bottom left middle
    //subcubes.push_back(Menger(min + 2.0f*thirdz, min + 2.0f*thirdz + third));                                                         // Bottom left back
    //subcubes.push_back(Menger(min + 2.0f*thirdz + thirdx, min + 2.0f*thirdz + thirdx + third));                                       // Bottom center back
    //subcubes.push_back(Menger(min + 2.0f*thirdz + 2.0f*thirdx, min + 2.0f*thirdz + 2.0f*thirdx + third));                             // Bottom right back
    //subcubes.push_back(Menger(min + thirdz + 2.0f*thirdx, min + thirdz + 2.0f*thirdx + third));                                       // Bottom right center

    //subcubes.push_back(Menger(min + thirdy, min + thirdy + third));                                                                   // Center left front
    //subcubes.push_back(Menger(min + thirdy + 2.0f*thirdz, min + thirdy + 2.0f*thirdz + third));                                       // Center left back
    //subcubes.push_back(Menger(min + thirdy + 2.0f*thirdz + 2.0f*thirdx, min + thirdy + 2.0f*thirdz + 2.0f*thirdx + third));           // Center right back
    //subcubes.push_back(Menger(min + thirdy + 2.0f*thirdx, min + thirdy + 2.0f*thirdx + third));                                       // Center right front

    //subcubes.push_back(Menger(min + 2.0f*thirdy, min + 2.0f*thirdy + third));                                                         // Top left front
    //subcubes.push_back(Menger(min + 2.0f*thirdy + thirdx, min + 2.0f*thirdy + thirdx + third));                                       // Top center front
    //subcubes.push_back(Menger(min + 2.0f*thirdy + 2.0f*thirdx, min + 2.0f*thirdy + 2.0f*thirdx + third));                             // Top right front
    //subcubes.push_back(Menger(min + 2.0f*thirdy + thirdz, min + 2.0f*thirdy + thirdz + third));                                       // Top left middle
    //subcubes.push_back(Menger(min + 2.0f*thirdy + 2.0f*thirdz, min + 2.0f*thirdy + 2.0f*thirdz + third));                             // Top left back
    //subcubes.push_back(Menger(min + 2.0f*thirdy + 2.0f*thirdz + thirdx, min + 2.0f*thirdy + 2.0f*thirdz + thirdx + third));           // Top center back
    //subcubes.push_back(Menger(min + 2.0f*thirdy + 2.0f*thirdz + 2.0f*thirdx, min + 2.0f*thirdy + 2.0f*thirdz + 2.0f*thirdx + third)); // Top right back
    //subcubes.push_back(Menger(min + 2.0f*thirdy + thirdz + 2.0f*thirdx, min + 2.0f*thirdy + thirdz + 2.0f*thirdx + third));           // Bottom right center

    for(auto it = subcubes.begin(); it != subcubes.end(); ++it) {
        it->set_nesting_level(nesting_level_ - 1);
        it->generate_geometry(obj_vertices, vtx_normals, obj_faces);
    }
}

static int count = 0;

void Menger::generate_cube(std::vector<glm::vec4>* obj_vertices, std::vector<glm::vec4>* vtx_normals,
                           std::vector<glm::uvec3>* obj_faces) const {
    std::cout << "Generate cube " << std::endl;
    unsigned long idx = obj_vertices->size();

    glm::vec4 v1(min.x, min.y, min.z, 1.0f);
    glm::vec4 v2(max.x, min.y, min.z, 1.0f);
    glm::vec4 v3(max.x, max.y, min.z, 1.0f);
    glm::vec4 v4(min.x, max.y, min.z, 1.0f);
    glm::vec4 v5(min.x, max.y, max.z, 1.0f);
    glm::vec4 v6(max.x, max.y, max.z, 1.0f);
    glm::vec4 v7(max.x, min.y, max.z, 1.0f);
    glm::vec4 v8(min.x, min.y, max.z, 1.0f);

    glm::vec4 n1(1.0f, 0.0f, 0.0f, 0.0f);
    glm::vec4 n2(0.0f, 1.0f, 0.0f, 0.0f);
    glm::vec4 n3(0.0f, 0.0f, 1.0f, 0.0f);

    obj_vertices->push_back(v8);
    vtx_normals->push_back(n3);
    obj_vertices->push_back(v7);
    vtx_normals->push_back(n3);
    obj_vertices->push_back(v6);
    vtx_normals->push_back(n3);
    obj_vertices->push_back(v5);
    vtx_normals->push_back(n3);
    obj_faces->push_back(glm::uvec3(idx, idx + 1, idx + 2));
    obj_faces->push_back(glm::uvec3(idx, idx + 2, idx + 3));


    //add_face(obj_vertices, vtx_normals, obj_faces, v1, v4, v3, v2, n3);   // Back face
    //add_face(obj_vertices, vtx_normals, obj_faces, v8, v7, v6, v5, n3);   // Front face
    //add_face(obj_vertices, vtx_normals, obj_faces, v7, v2, v3, v6, n2);   // Right face
}

void Menger::add_face(std::vector<glm::vec4>* obj_vertices, std::vector<glm::vec4>* vtx_normals,
                      std::vector<glm::uvec3>* obj_faces, glm::vec4 v1, glm::vec4 v2, glm::vec4 v3, glm::vec4 v4,
                      glm::vec4 normal) const {
    unsigned long idx = obj_vertices->size();
    obj_vertices->push_back(v1);
    vtx_normals->push_back(normal);
    obj_vertices->push_back(v2);
    vtx_normals->push_back(normal);
    obj_vertices->push_back(v3);
    vtx_normals->push_back(normal);
    obj_vertices->push_back(v4);
    vtx_normals->push_back(normal);

    obj_faces->push_back(glm::uvec3(idx, idx + 1, idx + 2));
    obj_faces->push_back(glm::uvec3(idx, idx + 2, idx + 3));

    std::cout << "Drawing front face: (" << v1.x << ", " << v1.y << ", " << v1.z << "), (" << v2.x << ", " << v2.y << ", " << v2.z << "), (" << v3.x << ", " << v3.y << ", " << v3.z << "), (" << v4.x << ", " << v4.y << ", " << v4.z << ") " << std::endl;
    std::cout << "Added: (" << idx << ", " << idx + 1 << ", " << idx + 2 << ") and (" << idx << ", " << idx + 2 << ", " << idx + 3 << ")" << std::endl;
}



