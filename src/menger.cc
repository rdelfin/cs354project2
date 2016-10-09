#include "menger.h"

namespace {
	const int kMinLevel = 0;
	const int kMaxLevel = 4;
};

Menger::Menger()
{
	// Add additional initialization if you like
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
    // Front face
    obj_vertices.push_back(glm::vec4(0.5f, -0.5f, -0.5f, 1.0f));
    vtx_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(0.5f, 0.5f, -0.5f, 1.0f));
    vtx_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(-0.5f, 0.5f, -0.5f, 1.0f));
    vtx_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f));
    vtx_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));

    obj_faces.push_back(glm::uvec3(0, 1, 2));
    obj_faces.push_back(glm::uvec3(0, 2, 3));

    // Back face
    obj_vertices.push_back(glm::vec4(0.5f, -0.5f, 0.5f, 1.0f));
    vtx_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    vtx_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f));
    vtx_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(-0.5f, -0.5f, 0.5f, 1.0f));
    vtx_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));

    obj_faces.push_back(glm::uvec3(4, 5, 6));
    obj_faces.push_back(glm::uvec3(4, 6, 7));

    // Right face
    obj_vertices.push_back(glm::vec4(0.5f, -0.5f, 0.5f, 1.0f));
    vtx_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    vtx_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(0.5f, 0.5f, -0.5f, 1.0f));
    vtx_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(0.5f, -0.5f, -0.5f, 1.0f));
    vtx_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));

    obj_faces.push_back(glm::uvec3(8, 9, 10));
    obj_faces.push_back(glm::uvec3(8, 10, 11));


    // Left face
    obj_vertices.push_back(glm::vec4(-0.5f, -0.5f, 0.5f, 1.0f));
    vtx_normals.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f));
    vtx_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(-0.5f, 0.5f, -0.5f, 1.0f));
    vtx_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f));
    vtx_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));

    obj_faces.push_back(glm::uvec3(12, 13, 14));
    obj_faces.push_back(glm::uvec3(12, 14, 15));

    // Top face
    obj_vertices.push_back(glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f));
    vtx_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(-0.5f, 0.5f, -0.5f, 1.0f));
    vtx_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(0.5f, 0.5f, -0.5f, 1.0f));
    vtx_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    vtx_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));

    obj_faces.push_back(glm::uvec3(16, 17, 18));
    obj_faces.push_back(glm::uvec3(16, 18, 19));

    // Bottom face
    obj_vertices.push_back(glm::vec4(-0.5f, -0.5f, 0.5f, 1.0f));
    vtx_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f));
    vtx_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(0.5f, -0.5f, -0.5f, 1.0f));
    vtx_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    obj_vertices.push_back(glm::vec4(0.5f, -0.5f, 0.5f, 1.0f));
    vtx_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));

    obj_faces.push_back(glm::uvec3(20, 21, 22));
    obj_faces.push_back(glm::uvec3(20, 22, 23));
}

