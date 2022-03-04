/*!
@file    glapp.cpp
@author  pghali@digipen.edu
@date    10/11/2016
@co-author : Dong-A Choi

This file implements functionality useful and necessary to build OpenGL
applications including use of external APIs such as GLFW to create a
window and start up an OpenGL context and to extract function pointers
to OpenGL implementations.

*//*__________________________________________________________________________*/

/*                                                                   includes
----------------------------------------------------------------------------- */
#include <glapp.h>
#include <glhelper.h>
#include <glslshader.h>
#include <array>

/*                                                   objects with file scope
----------------------------------------------------------------------------- */
static GLApp    g_glapp;
GLApp::GLModel GLApp::mdl;

void GLApp::init() {
	// Part 1: clear colorbuffer with RGBA value in glClearColor 
	glClearColor(1.f, 0.f, 0.f, 1.f);
	
	// Part 2: use the entire window as viewport 
	GLint w = GLHelper::width, h = GLHelper::height;
	glViewport(0,0,w,h);

	
	// Part 3: initialize VAO and create shader program
	mdl.setup_vao();
	mdl.setup_shdrpgm();
	
	// Part 4: This is related to Task 1 where OpenGL context

	glViewport(0, 0, GLHelper::width, GLHelper::height);

	GLubyte const* str_ven = glGetString(GL_VENDOR);
	std::cout << "GPU Vendor: " << str_ven << std::endl;

	GLubyte const* str_ren = glGetString(GL_RENDERER);
	std::cout << "GL Renderer: " << str_ren << std::endl;

	GLubyte const* str_ver = glGetString(GL_VERSION);
	std::cout << "GL Version: " << str_ver << std::endl;

	GLubyte const* sha_ver = glGetString(GL_SHADING_LANGUAGE_VERSION);
    std::cout << "GL Shader Version: " << sha_ver << std::endl;

	GLint major_num;
	glGetIntegerv(GL_MAJOR_VERSION, &major_num);
	std::cout << "GL Major Version: " << major_num << std::endl;

	GLint minor_num;
	glGetIntegerv(GL_MINOR_VERSION, &minor_num);
	std::cout << "GL Minor Version: " << minor_num << std::endl;

	GLint d_buffer;
	glGetIntegerv(GL_DOUBLEBUFFER, &d_buffer);
	if (d_buffer == 1) {
		std::cout << "Current OpenGL Context is double buffered" << std::endl;
	}

	GLint max_ver;
	glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &max_ver);
	std::cout << "Maximum Vertex Count: " << max_ver << std::endl;

	GLint max_ind;
	glGetIntegerv(GL_MAX_ELEMENTS_INDICES, &max_ind);
	std::cout << "Maximum Indices Count: " << max_ind << std::endl;

	GLint tex_size;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &tex_size);
	std::cout << "Maximum texture size: " << tex_size << std::endl;

	GLint max_view[2];
	glGetIntegerv(GL_MAX_VIEWPORT_DIMS, &max_view[0]);
	std::cout << "Maximum Viewport Dimensions: " << *max_view << " x " << *max_view << std::endl;
}

void GLApp::update(double delta_time) {
	GLfloat color = static_cast<float>(sin(glfwGetTime())) * 0.5f;
	glClearColor(color,color, 0.f, 1.f);
	
}

void GLApp::draw() {
	glClear(GL_COLOR_BUFFER_BIT);

	mdl.draw();
}

void GLApp::cleanup() {
  // empty for now
}

void GLApp::GLModel::setup_vao()
{
	// We'll define a rectangle in normalized device coordinates (NDC)
// coordinates that has one-fourth the area of the window.
// The NDC coordinates for a window range from [-1, 1] along both
// both the X- and Y-axes. Therefore, the rectangle's (x, y) position
// coordinates are in range [-0.5, 0.5]
// We're using NDC coordinates, because we don't want to specify
// a "model-to-world-to-view-to-clip" transform to the vertex shader.
	std::array<glm::vec2, 4> pos_vtx{
	glm::vec2(0.5f, -0.5f), glm::vec2(0.5f, 0.5f),
	glm::vec2(-0.5f, 0.5f), glm::vec2(-0.5f, -0.5f)
	};
	std::array<glm::vec3, 4> clr_vtx{
	glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f),
	glm::vec3(0.f, 0.f, 1.f), glm::vec3(1.f, 1.f, 1.f)
	};

	GLuint vbo_hdl;
	glCreateBuffers(1, &vbo_hdl);

	glNamedBufferStorage(vbo_hdl,
		sizeof(glm::vec2) * pos_vtx.size() + sizeof(glm::vec3) * clr_vtx.size(),
		nullptr, GL_DYNAMIC_STORAGE_BIT);
	glNamedBufferSubData(vbo_hdl, 0,
		sizeof(glm::vec2) * pos_vtx.size(), pos_vtx.data());
	glNamedBufferSubData(vbo_hdl, sizeof(glm::vec2) * pos_vtx.size(),
		sizeof(glm::vec3) * clr_vtx.size(), clr_vtx.data());

	GLint max_vtx_attribs;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_vtx_attribs);
	std::cout << "Maximum vertex attributes: " << max_vtx_attribs << '\n';

	glCreateVertexArrays(1, &vaoid); 
	// for vertex position array, we use vertex attribute index 8
	// and vertex buffer binding point 3
	glEnableVertexArrayAttrib(vaoid, 8);
	glVertexArrayVertexBuffer(vaoid, 3, vbo_hdl, 0, sizeof(glm::vec2));
	glVertexArrayAttribFormat(vaoid, 8, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vaoid, 8, 3);

	// for vertex color array, we use vertex attribute index 9
	// and vertex buffer binding point 4
	glEnableVertexArrayAttrib(vaoid, 9);
	glVertexArrayVertexBuffer(vaoid, 4, vbo_hdl,
		sizeof(glm::vec2) * pos_vtx.size(), sizeof(glm::vec3));
	glVertexArrayAttribFormat(vaoid, 9, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vaoid, 9, 4);

	primitive_type = GL_TRIANGLES;
	std::array<GLushort, 6> idx_vtx{ 0, 1, 2, 2, 3, 0 };
	idx_elem_cnt = static_cast<GLuint>(idx_vtx.size());
	GLuint ebo_hdl;
	glCreateBuffers(1, &ebo_hdl);
	glNamedBufferStorage(ebo_hdl, sizeof(GLushort) * idx_elem_cnt,
		reinterpret_cast<GLvoid*>(idx_vtx.data()),
		GL_DYNAMIC_STORAGE_BIT);
	glVertexArrayElementBuffer(vaoid, ebo_hdl);
	glBindVertexArray(0);

}

void GLApp::GLModel::setup_shdrpgm() {
	std::vector<std::pair<GLenum, std::string>> shdr_files;
	shdr_files.push_back(std::make_pair(
		GL_VERTEX_SHADER,
		"../shaders/my-tutorial-1.vert"));
	shdr_files.push_back(std::make_pair(
		GL_FRAGMENT_SHADER,
		"../shaders/my-tutorial-1.frag"));
	shdr_pgm.CompileLinkValidate(shdr_files);
	if (GL_FALSE == shdr_pgm.IsLinked()) {
		std::cout << "Unable to compile/link/validate shader programs" << "\n";
		std::cout << shdr_pgm.GetLog() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

void GLApp::GLModel::draw() {
	// there are many shader programs initialized - here we're saying
	// which specific shader program should be used to render geometry
	shdr_pgm.Use();
	// there are many models, each with their own initialized VAO object
	// here, we're saying which VAO's state should be used to set up pipe
	glBindVertexArray(vaoid);
	// here, we're saying what primitive is to be rendered and how many
	// such primitives exist.
	// the graphics driver knows where to get the indices because the VAO
	// containing this state information has been made current ...
	glDrawElements(primitive_type, idx_elem_cnt, GL_UNSIGNED_SHORT, NULL);
	// after completing the rendering, we tell the driver that VAO
	// vaoid and current shader program are no longer current
	glBindVertexArray(0);
	shdr_pgm.UnUse();
}






