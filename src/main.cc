#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

// OpenGL library includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <debuggl.h>
#include "menger.h"
#include "camera.h"

int window_width = 800, window_height = 600;

// VBO and VAO descriptors.
enum { kVertexBuffer, kNormalBuffer, kIndexBuffer, kNumVbos };

// These are our VAOs.
enum { kGeometryVao, kFloorVao, kNumVaos };

GLuint g_array_objects[kNumVaos];  // This will store the VAO descriptors.
GLuint g_buffer_objects[kNumVaos][kNumVbos];  // These will store VBO descriptors.

// C++ 11 String Literal
// See http://en.cppreference.com/w/cpp/language/string_literal
const char* vertex_shader =
R"zzz(#version 330 core
in vec4 vertex_position;
in vec4 vertex_normal;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 light_position;
out vec4 light_direction;
out vec4 normal;
out vec4 world_normal;
out vec4 world_position;
void main()
{
// Transform vertex into clipping coordinates
	world_position =  vertex_position;
    gl_Position = projection * view * world_position;
// Lighting in camera coordinates
//  Compute light direction and transform to camera coordinates
        light_direction = view * (light_position - vertex_position);
//  Transform normal to camera coordinates
        normal = view * vertex_normal;
        world_normal = vertex_normal;
}
)zzz";

const char* fragment_shader =
R"zzz(#version 330 core
in vec4 normal;
in vec4 light_direction;
in vec4 world_normal;
out vec4 fragment_color;
void main()
{
    vec4 color = vec4(1.0*world_normal.x, 1.0*world_normal.y, 1.0*world_normal.z, 1.0);
    float dot_nl = dot(normalize(light_direction), normalize(normal));
    dot_nl = clamp(dot_nl, 0.0, 1.0);
    fragment_color = clamp(dot_nl * color, 0.0, 1.0);
}
)zzz";

// FIXME: Implement shader effects with an alternative shader.
const char* floor_fragment_shader =
R"zzz(#version 330 core
in vec4 normal;
in vec4 light_direction;
in vec4 world_position;
out vec4 fragment_color;

void main()
{
    vec4 color = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 modded_coord = mod(world_position, vec4(0.5));
    if(modded_coord.x > 0.25) {
        if(modded_coord.z > 0.25) {
            color = vec4(1.0, 1.0, 1.0, 1.0);
        } else {
            color = vec4(0.0, 0.0, 0.0, 1.0);
        }
    } else {
        if(modded_coord.z > 0.25) {
            color = vec4(0.0, 0.0, 0.0, 1.0);
        } else {
            color = vec4(1.0, 1.0, 1.0, 1.0);
        }
    }

    float dot_nl = dot(normalize(light_direction), normalize(normal));
    dot_nl = clamp(dot_nl, 0.0, 1.0);
    fragment_color = clamp(dot_nl * color, 0.0, 1.0);
}
)zzz";

void
ErrorCallback(int error, const char* description)
{
	std::cerr << "GLFW Error: " << description << "\n";
}

std::shared_ptr<Menger> g_menger;
Camera g_camera;

bool g_ctrl_pressed;
bool g_shift_pressed;
bool g_alt_pressed;
bool fps = false;

void
KeyCallback(GLFWwindow* window,
            int key,
            int scancode,
            int action,
            int mods) {
    // Note:
    // This is only a list of functions to implement.
    // you may want to re-organize this piece of code.
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    } else if (key == GLFW_KEY_W && action != GLFW_RELEASE) {
        if(fps) g_camera.pan(glm::vec3(0, 0, 1));
		else g_camera.zoom(-1);
    } else if (key == GLFW_KEY_S && action != GLFW_RELEASE) {
		if(fps) g_camera.pan(glm::vec3(0, 0, -1));
		else g_camera.zoom(1);
    } else if (key == GLFW_KEY_A && action != GLFW_RELEASE) {
        if(fps) g_camera.pan(glm::vec3(-1, 0, 0));
		else g_camera.strave(glm::vec3(-1, 0, 0));
    } else if (key == GLFW_KEY_D && action != GLFW_RELEASE) {
		if(fps) g_camera.pan(glm::vec3(1, 0, 0));
		else g_camera.strave(glm::vec3(1, 0, 0));
    } else if (key == GLFW_KEY_LEFT && action != GLFW_RELEASE) {
        g_camera.roll(-1);
    } else if (key == GLFW_KEY_RIGHT && action != GLFW_RELEASE) {
		g_camera.roll(1);
    } else if (key == GLFW_KEY_DOWN && action != GLFW_RELEASE) {
		if(fps) g_camera.pan(glm::vec3(0, -1, 0));
		else g_camera.strave(glm::vec3(0, -1, 0));
    } else if (key == GLFW_KEY_UP && action != GLFW_RELEASE) {
		if(fps) g_camera.pan(glm::vec3(0, 1, 0));
		else g_camera.strave(glm::vec3(0, 1, 0));
    } else if (key == GLFW_KEY_C && action != GLFW_RELEASE) {
		fps  = !fps;
		std::cout << "FPS: " << fps << std::endl;
    }
    if (!g_menger)
        return ; // 0-4 only available in Menger mode.
    if (key == GLFW_KEY_0 && action != GLFW_RELEASE) {
		g_menger->set_nesting_level(0);
    } else if (key == GLFW_KEY_1 && action != GLFW_RELEASE) {
		g_menger->set_nesting_level(1);
    } else if (key == GLFW_KEY_2 && action != GLFW_RELEASE) {
		g_menger->set_nesting_level(2);
    } else if (key == GLFW_KEY_3 && action != GLFW_RELEASE) {
		g_menger->set_nesting_level(3);
    } else if (key == GLFW_KEY_4 && action != GLFW_RELEASE) {
		g_menger->set_nesting_level(4);
    }


    // Used for ALT, CTRL and SHIFT
    if(action == GLFW_PRESS || action == GLFW_RELEASE) {
        if (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL) {
            g_ctrl_pressed = action == GLFW_PRESS;
        } else if (key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT) {
            g_shift_pressed = action == GLFW_PRESS;
        } else if (key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT) {
            g_alt_pressed = action == GLFW_PRESS;
        }
    }
}

int g_current_button;
bool g_prev_mouse_pressed;
bool g_mouse_pressed;
glm::vec2 prevMouse(0.0f, 0.0f);

void generate_floor(std::vector<glm::vec4> &vertices, std::vector<glm::vec4> &normals, std::vector<glm::uvec3> &faces);

void
MousePosCallback(GLFWwindow* window, double mouse_x, double mouse_y)
{
    glm::vec2 mouse(mouse_x, mouse_y);
    glm::vec2 deltaMouse = mouse - prevMouse;

	if (g_mouse_pressed && g_prev_mouse_pressed) {
        if (g_current_button == GLFW_MOUSE_BUTTON_LEFT && !g_alt_pressed && !g_shift_pressed && !g_ctrl_pressed) {
            g_camera.pitch((180.0f / M_PI) * -deltaMouse.y / window_width);
            g_camera.yaw((180.0f /  M_PI) * -deltaMouse.x / window_height);
        } else if (g_current_button == GLFW_MOUSE_BUTTON_RIGHT || (g_current_button == GLFW_MOUSE_BUTTON_LEFT && (g_alt_pressed || g_shift_pressed))) {
            g_camera.zoom(10.0f * deltaMouse.y / window_height);
        } else if (g_current_button == GLFW_MOUSE_BUTTON_MIDDLE || (g_current_button == GLFW_MOUSE_BUTTON_LEFT && g_ctrl_pressed)) {
            g_camera.pan(25.0f*glm::vec3(-deltaMouse.x / window_width, deltaMouse.y / window_height, 0));
        }
    }

    prevMouse = glm::vec2(mouse_x, mouse_y);
    g_prev_mouse_pressed = g_mouse_pressed;
}

void
MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	g_mouse_pressed = (action == GLFW_PRESS);
	g_current_button = button;
}

int main(int argc, char* argv[])
{
	std::string window_title = "Menger";
	if (!glfwInit()) exit(EXIT_FAILURE);
	g_menger = std::make_shared<Menger>(glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0.5, 0.5, 0.5));
	glfwSetErrorCallback(ErrorCallback);

	// Ask an OpenGL 3.3 core profile context 
	// It is required on OSX and non-NVIDIA Linux
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(window_width, window_height,
			&window_title[0], nullptr, nullptr);
	CHECK_SUCCESS(window != nullptr);
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	CHECK_SUCCESS(glewInit() == GLEW_OK);
	glGetError();  // clear GLEW's error for it
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MousePosCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSwapInterval(1);
	const GLubyte* renderer = glGetString(GL_RENDERER);  // get renderer string
	const GLubyte* version = glGetString(GL_VERSION);    // version as a string
	std::cout << "Renderer: " << renderer << "\n";
	std::cout << "OpenGL version supported:" << version << "\n";

	std::vector<glm::vec4> obj_vertices;
	std::vector<glm::vec4> vtx_normals;
	std::vector<glm::uvec3> obj_faces;

	g_menger->set_nesting_level(4);
	g_menger->generate_geometry(obj_vertices, vtx_normals, obj_faces);
	g_menger->set_clean();

	glm::vec4 min_bounds = glm::vec4(std::numeric_limits<float>::max());
	glm::vec4 max_bounds = glm::vec4(-std::numeric_limits<float>::max());
	for (unsigned i = 0; i < obj_vertices.size(); ++i) {
		min_bounds = glm::min(obj_vertices[i], min_bounds);
		max_bounds = glm::max(obj_vertices[i], max_bounds);
	}
	std::cout << "min_bounds = " << glm::to_string(min_bounds) << "\n";
	std::cout << "max_bounds = " << glm::to_string(max_bounds) << "\n";

    /*===================================================================================
 	 *======================= GLM LOADING VBO AND VAO FOR MENGER ========================
	 *===================================================================================*/

	// Setup our VAO array.
	CHECK_GL_ERROR(glGenVertexArrays(kNumVaos, &g_array_objects[0]));

	// Switch to the VAO for Geometry.
	CHECK_GL_ERROR(glBindVertexArray(g_array_objects[kGeometryVao]));

	// Generate buffer objects
	CHECK_GL_ERROR(glGenBuffers(kNumVbos, &g_buffer_objects[kGeometryVao][0]));

	// Setup vertex data in a VBO.
	CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, g_buffer_objects[kGeometryVao][kVertexBuffer]));
	// NOTE: We do not send anything right now, we just describe it to OpenGL.
	CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER,
				sizeof(float) * obj_vertices.size() * 4, nullptr,
				GL_STATIC_DRAW));
	CHECK_GL_ERROR(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0));
	CHECK_GL_ERROR(glEnableVertexAttribArray(0));

	CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, g_buffer_objects[kGeometryVao][kNormalBuffer]));
	// NOTE: We do not send anything right now, we just describe it to OpenGL.
	CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER,
				sizeof(float) * vtx_normals.size() * 4, nullptr,
				GL_STATIC_DRAW));
	CHECK_GL_ERROR(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0));
	CHECK_GL_ERROR(glEnableVertexAttribArray(1));

	// Setup element array buffer.
	CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_buffer_objects[kGeometryVao][kIndexBuffer]));
	CHECK_GL_ERROR(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				sizeof(uint32_t) * obj_faces.size() * 3,
				&obj_faces[0], GL_STATIC_DRAW));



    /*===================================================================================
     *======================= GLM LOADING VBO AND VAO FOR FLOOR =========================
     *===================================================================================*/

    std::vector<glm::vec4> floor_vertices;
    std::vector<glm::vec4> floor_normals;
    std::vector<glm::uvec3> floor_faces;

    generate_floor(floor_vertices, floor_normals, floor_faces);

    std::cout << "VERTICES:" << std::endl;
    for(glm::vec4 vtx : floor_vertices)
        std::cout << "\t(" << vtx.x << ", " << vtx.y << ", " << vtx.z << ", " << vtx.w << ")" << std::endl;
    std::cout << "NORMALS:" << std::endl;
    for(glm::vec4 vtx : floor_normals)
        std::cout << "\t(" << vtx.x << ", " << vtx.y << ", " << vtx.z << ", " << vtx.w << ")" << std::endl;
    std::cout << "FACES:" << std::endl;
    for(glm::uvec3 vtx : floor_faces)
        std::cout << "\t(" << vtx.x << ", " << vtx.y << ", " << vtx.z << ")" << std::endl;

    // Setup our VAO array.
	CHECK_GL_ERROR(glBindVertexArray(g_array_objects[kFloorVao]));

    // Generate buffer objects
	CHECK_GL_ERROR(glGenBuffers(kNumVbos, &g_buffer_objects[kFloorVao][0]));

    // Setup vertex data in a VBO.
    CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, g_buffer_objects[kFloorVao][kVertexBuffer]));
    // NOTE: We do not send anything right now, we just describe it to OpenGL.
    CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER,
                                sizeof(float) * floor_vertices.size() * 4, nullptr,
                                GL_STATIC_DRAW));
    CHECK_GL_ERROR(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0));
    CHECK_GL_ERROR(glEnableVertexAttribArray(0));

    CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, g_buffer_objects[kFloorVao][kNormalBuffer]));
    // NOTE: We do not send anything right now, we just describe it to OpenGL.
    CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER,
                                sizeof(float) * floor_normals.size() * 4, nullptr,
                                GL_STATIC_DRAW));
    CHECK_GL_ERROR(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0));
    CHECK_GL_ERROR(glEnableVertexAttribArray(1));

    // Setup element array buffer.
    CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_buffer_objects[kFloorVao][kIndexBuffer]));
    CHECK_GL_ERROR(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                                sizeof(uint32_t) * floor_faces.size() * 3,
                                &floor_faces[0], GL_STATIC_DRAW));




	// Setup vertex shader.
	GLuint vertex_shader_id = 0;
	const char* vertex_source_pointer = vertex_shader;
	CHECK_GL_ERROR(vertex_shader_id = glCreateShader(GL_VERTEX_SHADER));
	CHECK_GL_ERROR(glShaderSource(vertex_shader_id, 1, &vertex_source_pointer, nullptr));
	glCompileShader(vertex_shader_id);
	CHECK_GL_SHADER_ERROR(vertex_shader_id);

	// Setup fragment shader.
	GLuint fragment_shader_id = 0;
	const char* fragment_source_pointer = fragment_shader;
	CHECK_GL_ERROR(fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER));
	CHECK_GL_ERROR(glShaderSource(fragment_shader_id, 1, &fragment_source_pointer, nullptr));
	glCompileShader(fragment_shader_id);
	CHECK_GL_SHADER_ERROR(fragment_shader_id);

	// Let's create our program.
	GLuint program_id = 0;
	CHECK_GL_ERROR(program_id = glCreateProgram());
	CHECK_GL_ERROR(glAttachShader(program_id, vertex_shader_id));
	CHECK_GL_ERROR(glAttachShader(program_id, fragment_shader_id));
//	CHECK_GL_ERROR(glAttachShader(program_id, geometry_shader_id));

	CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, g_buffer_objects[kGeometryVao][kVertexBuffer]));
	// NOTE: We do not send anything right now, we just describe it to OpenGL.
	CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER,
				sizeof(float) * obj_vertices.size() * 4, nullptr,
				GL_STATIC_DRAW));
	CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, g_buffer_objects[kGeometryVao][kNormalBuffer]));
	// NOTE: We do not send anything right now, we just describe it to OpenGL.
	CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER,
				sizeof(float) * vtx_normals.size() * 4, nullptr,
				GL_STATIC_DRAW));
	// Bind attributes.
	CHECK_GL_ERROR(glBindAttribLocation(program_id, 0, "vertex_position"));

	CHECK_GL_ERROR(glBindAttribLocation(program_id, 1, "vertex_normal"));
	CHECK_GL_ERROR(glBindFragDataLocation(program_id, 0, "fragment_color"));
	glLinkProgram(program_id);
	CHECK_GL_PROGRAM_ERROR(program_id);



	// Get the uniform locations.
	GLint projection_matrix_location = 0;
	CHECK_GL_ERROR(projection_matrix_location =
			glGetUniformLocation(program_id, "projection"));
	GLint view_matrix_location = 0;
	CHECK_GL_ERROR(view_matrix_location =
			glGetUniformLocation(program_id, "view"));
	GLint light_position_location = 0;
	CHECK_GL_ERROR(light_position_location =
			glGetUniformLocation(program_id, "light_position"));

	// Setup fragment shader for the floor
	GLuint floor_fragment_shader_id = 0;
	const char* floor_fragment_source_pointer = floor_fragment_shader;
	CHECK_GL_ERROR(floor_fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER));
	CHECK_GL_ERROR(glShaderSource(floor_fragment_shader_id, 1,
				&floor_fragment_source_pointer, nullptr));
	glCompileShader(floor_fragment_shader_id);
	CHECK_GL_SHADER_ERROR(floor_fragment_shader_id);




	// Code for initializing the floor GLSL program
	GLuint floor_program_id = 0;
	GLint floor_projection_matrix_location = 0;
	GLint floor_view_matrix_location = 0;
	GLint floor_light_position_location = 0;

    CHECK_GL_ERROR(floor_program_id = glCreateProgram());
    CHECK_GL_ERROR(glAttachShader(floor_program_id, vertex_shader_id));
    CHECK_GL_ERROR(glAttachShader(floor_program_id, floor_fragment_shader_id));
    CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, g_buffer_objects[kFloorVao][kVertexBuffer]));
    CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER,
                                sizeof(float) * floor_vertices.size() * 4, nullptr,
                                GL_STATIC_DRAW));
    CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, g_buffer_objects[kFloorVao][kNormalBuffer]));
    CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER,
                                sizeof(float) * floor_normals.size() * 4, nullptr,
                                GL_STATIC_DRAW));
    CHECK_GL_ERROR(glBindAttribLocation(floor_program_id, 0, "vertex_position"));

    CHECK_GL_ERROR(glBindAttribLocation(floor_program_id, 1, "vertex_normal"));
    CHECK_GL_ERROR(glBindFragDataLocation(floor_program_id, 0, "fragment_color"));
    glLinkProgram(floor_program_id);
    CHECK_GL_PROGRAM_ERROR(floor_program_id);
    CHECK_GL_ERROR(floor_projection_matrix_location =
                           glGetUniformLocation(floor_program_id, "projection"));
    CHECK_GL_ERROR(floor_view_matrix_location =
                           glGetUniformLocation(floor_program_id, "view"));
    CHECK_GL_ERROR(floor_light_position_location =
                           glGetUniformLocation(floor_program_id, "light_position"));



	glm::vec4 light_position = glm::vec4(2.0f, 2.0f, 2.0f, 1.0f);
	float aspect = 0.0f;
	float theta = 0.0f;
	while (!glfwWindowShouldClose(window)) {
		// Setup some basic window stuff.
		glfwGetFramebufferSize(window, &window_width, &window_height);
		glViewport(0, 0, window_width, window_height);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDepthFunc(GL_LESS);

		// Switch to the Geometry VAO.
		CHECK_GL_ERROR(glBindVertexArray(g_array_objects[kGeometryVao]));

		if (g_menger && g_menger->is_dirty()) {
            obj_vertices.clear();
            vtx_normals.clear();
            obj_faces.clear();
		    g_menger->generate_geometry(obj_vertices, vtx_normals, obj_faces);
            std::cout << "Number of vertices: " << obj_vertices.size() << std::endl;
			g_menger->set_clean();
		}

		// Compute the projection matrix.
		aspect = static_cast<float>(window_width) / window_height;
		glm::mat4 projection_matrix =
			glm::perspective(glm::radians(45.0f), aspect, 0.0001f, 1000.0f);

		// Compute the view matrix
		// FIXME: change eye and center through mouse/keyboard events.
		glm::mat4 view_matrix = g_camera.get_view_matrix();

		// Send vertices to the GPU.
		CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER,
		                            g_buffer_objects[kGeometryVao][kVertexBuffer]));
		CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER,
		                            sizeof(float) * obj_vertices.size() * 4,
		                            &obj_vertices[0], GL_STATIC_DRAW));
		CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER,
		                            g_buffer_objects[kGeometryVao][kNormalBuffer]));
		CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER,
		                            sizeof(float) * vtx_normals.size() * 4,
		                            &vtx_normals[0], GL_STATIC_DRAW));
		// Use our program.
		CHECK_GL_ERROR(glUseProgram(program_id));

		// Pass uniforms in.
		CHECK_GL_ERROR(glUniformMatrix4fv(projection_matrix_location, 1, GL_FALSE,
					&projection_matrix[0][0]));
		CHECK_GL_ERROR(glUniformMatrix4fv(view_matrix_location, 1, GL_FALSE,
					&view_matrix[0][0]));
		CHECK_GL_ERROR(glUniform4fv(light_position_location, 1, &light_position[0]));

		// Draw our triangles.
		CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES, obj_faces.size() * 3, GL_UNSIGNED_INT, 0));


        CHECK_GL_ERROR(glBindVertexArray(g_array_objects[kFloorVao]));

        // Send vertices to the GPU.
        CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER,
                                    g_buffer_objects[kFloorVao][kVertexBuffer]));
        CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER,
                                    sizeof(float) * floor_vertices.size() * 4,
                                    &floor_vertices[0], GL_STATIC_DRAW));
        CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER,
                                    g_buffer_objects[kFloorVao][kNormalBuffer]));
        CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER,
                                    sizeof(float) * floor_normals.size() * 4,
                                    &floor_normals[0], GL_STATIC_DRAW));
        // Use our program.
        CHECK_GL_ERROR(glUseProgram(floor_program_id));

        // Pass uniforms in.
        CHECK_GL_ERROR(glUniformMatrix4fv(floor_projection_matrix_location, 1, GL_FALSE,
                                          &projection_matrix[0][0]));
        CHECK_GL_ERROR(glUniformMatrix4fv(floor_view_matrix_location, 1, GL_FALSE,
                                          &view_matrix[0][0]));
        CHECK_GL_ERROR(glUniform4fv(floor_light_position_location, 1, &light_position[0]));

        // Draw our triangles.
        CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES, floor_faces.size() * 3, GL_UNSIGNED_INT, 0));

		// Poll and swap.
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void generate_floor(std::vector<glm::vec4> &vertices, std::vector<glm::vec4> &normals, std::vector<glm::uvec3> &faces) {
    unsigned long idx = vertices.size();

	// CLOSE ENOUGH TO INFINITE
    vertices.push_back(glm::vec4(100,  -2, 100,  1));
    vertices.push_back(glm::vec4(100,  -2, -100, 1));
    vertices.push_back(glm::vec4(-100, -2, -100, 1));
    vertices.push_back(glm::vec4(-100, -2, 100,  1));

    normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));

    faces.push_back(glm::uvec3(idx, idx + 1, idx + 2));
    faces.push_back(glm::uvec3(idx, idx + 2, idx + 3));
}
