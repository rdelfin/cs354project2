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
        generate_cube(obj_vertices, vtx_normals, obj_faces, glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0.5, 0.5, 0.5));
    }

    std::vector<Menger> subcubes;

    glm::vec3 third = min + (max - min) / 3.0f;
    glm::vec3 thirdx = glm::vec3(thirdx.x, 0, 0);
    glm::vec3 thirdy = glm::vec3(0, thirdx.y, 0);
    glm::vec3 thirdz = glm::vec3(0, 0, thirdx.z);

    subcubes.push_back(Menger(min, min + third));                                                                                     // Bottom left front
    subcubes.push_back(Menger(min + thirdx, min + thirdx + third));                                                                   // Bottom center front
    subcubes.push_back(Menger(min + 2.0f*thirdx, min + 2.0f*thirdx + third));                                                         // Bottom right front
    subcubes.push_back(Menger(min + thirdz, min + thirdz + third));                                                                   // Bottom left middle
    subcubes.push_back(Menger(min + 2.0f*thirdz, min + 2.0f*thirdz + third));                                                         // Bottom left back
    subcubes.push_back(Menger(min + 2.0f*thirdz + thirdx, min + 2.0f*thirdz + thirdx + third));                                       // Bottom center back
    subcubes.push_back(Menger(min + 2.0f*thirdz + 2.0f*thirdx, min + 2.0f*thirdz + 2.0f*thirdx + third));                             // Bottom right back

    subcubes.push_back(Menger(min + thirdy, min + thirdy + third));                                                                   // Center left front
    subcubes.push_back(Menger(min + thirdy + 2.0f*thirdz, min + thirdy + 2.0f*thirdz + third));                                       // Center left back
    subcubes.push_back(Menger(min + thirdy + 2.0f*thirdz + 2.0f*thirdx, min + thirdy + 2.0f*thirdz + 2.0f*thirdx + third));           // Center right back
    subcubes.push_back(Menger(min + thirdy + 2.0f*thirdx, min + thirdy + 2.0f*thirdx + third));                                       // Center right front

    subcubes.push_back(Menger(min + 2.0f*thirdy, min + 2.0f*thirdy + third));                                                         // Top left front
    subcubes.push_back(Menger(min + 2.0f*thirdy + thirdx, min + 2.0f*thirdy + thirdx + third));                                       // Top center front
    subcubes.push_back(Menger(min + 2.0f*thirdy + 2.0f*thirdx, min + 2.0f*thirdy + 2.0f*thirdx + third));                             // Top right front
    subcubes.push_back(Menger(min + 2.0f*thirdy + thirdz, min + 2.0f*thirdy + thirdz + third));                                       // Top left middle
    subcubes.push_back(Menger(min + 2.0f*thirdy + 2.0f*thirdz, min + 2.0f*thirdy + 2.0f*thirdz + third));                             // Top left back
    subcubes.push_back(Menger(min + 2.0f*thirdy + 2.0f*thirdz + thirdx, min + 2.0f*thirdy + 2.0f*thirdz + thirdx + third));           // Top center back
    subcubes.push_back(Menger(min + 2.0f*thirdy + 2.0f*thirdz + 2.0f*thirdx, min + 2.0f*thirdy + 2.0f*thirdz + 2.0f*thirdx + third)); // Top right back

    for(auto it = subcubes.begin(); it != subcubes.end(); ++it) {
        it->generate_geometry(obj_vertices, vtx_normals, obj_faces);
    }

}

void Menger::generate_cube(std::vector<glm::vec4> &obj_vertices, std::vector<glm::vec4> &vtx_normals,
                           std::vector<glm::uvec3> &obj_faces, glm::vec3 min, glm::vec3 max) const {
    unsigned idx = obj_vertices.size();
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

    obj_faces.push_back(glm::uvec3(4, 5, 6));
    obj_faces.push_back(glm::uvec3(4, 6, 7));

    // Right face
    obj_vertices.push_back(glm::vec4(max.x, min.y, max.z, 1.0f));
    vtx_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(max.x, max.y, max.z, 1.0f));
    vtx_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(max.x, 0.5f, -0.5f, 1.0f));
    vtx_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(max.x, min.y, min.z, 1.0f));
    vtx_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));

    obj_faces.push_back(glm::uvec3(8, 9, 10));
    obj_faces.push_back(glm::uvec3(8, 10, 11));


    // Left face
    obj_vertices.push_back(glm::vec4(min.x, min.y, max.z, 1.0f));
    vtx_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(min.x, max.y, max.z, 1.0f));
    vtx_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(min.x, max.y, min.z, 1.0f));
    vtx_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(min.x, min.y, min.z, 1.0f));
    vtx_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));

    obj_faces.push_back(glm::uvec3(12, 13, 14));
    obj_faces.push_back(glm::uvec3(12, 14, 15));

    // Top face
    obj_vertices.push_back(glm::vec4(min.x, max.y, max.z, 1.0f));
    vtx_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(min.x, max.y, min.z, 1.0f));
    vtx_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(max.x, max.y, min.z, 1.0f));
    vtx_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(max.x, max.y, max.z, 1.0f));
    vtx_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));

    obj_faces.push_back(glm::uvec3(16, 17, 18));
    obj_faces.push_back(glm::uvec3(16, 18, 19));

    // Bottom face
    obj_vertices.push_back(glm::vec4(min.x, min.y, max.z, 1.0f));
    vtx_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(min.x, min.y, min.z, 1.0f));
    vtx_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(max.x, min.y, min.z, 1.0f));
    vtx_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(max.x, min.y, max.z, 1.0f));
    vtx_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));

    obj_faces.push_back(glm::uvec3(20, 21, 22));
    obj_faces.push_back(glm::uvec3(20, 22, 23));
}



