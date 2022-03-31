/*!
@file    glapp.cpp
@author  pghali@digipen.edu
@date    10/11/2016
@co-author : Dong-A Choi, Sunwoo Lee

// Dong-A Choi, Sunwoo Lee
// CS250 Class Project
// CS250
// 2022 spring

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
#include <IG.h>

#include "math.hpp"

/*                                                   objects with file scope
----------------------------------------------------------------------------- */
//static GLApp    g_glapp;
GLApp::GLModel GLApp::mdl;

void GLApp::init() {
	GLint w = GLHelper::width, h = GLHelper::height;
	glViewport(0,0,w,h);

	mdl.setup_vao();
	mdl.setup_shdrpgm();

	GLubyte const* str_ven = glGetString(GL_VENDOR);
	std::cout << "GPU Vendor: " << str_ven << std::endl;

	GLubyte const* str_ren = glGetString(GL_RENDERER);
	std::cout << "GL Renderer: " << str_ren << std::endl;

	GLubyte const* str_ver = glGetString(GL_VERSION);
	std::cout << "GL Version: " << str_ver << std::endl;

	GLubyte const* sha_ver = glGetString(GL_SHADING_LANGUAGE_VERSION);
    std::cout << "GL Shader Version: " << sha_ver << std::endl;

	IG::init();
}

void GLApp::update(double delta_time) {
	//glClearColor(0.f,0.f, 0.f, 1.f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	IG::update();
}

void GLApp::draw() {  
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mdl.draw();
	IG::draw();
}

void GLApp::cleanup() {
  // empty for now
	IG::cleanup();
}

void GLApp::GLModel::setup_vao()
{
	/*rot = glm::rotate(rot, angle, glm::vec3(x_axis, y_axis, z_axis));
	GLint rotPostion  = glGetUniformLocation(shdr_pgm.GetHandle(), "rotateMatrix");
	glUniformMatrix4fv(rotPostion, 1, GL_FALSE, glm::value_ptr(rot));*/
	glEnable(GL_DEPTH_TEST);

	std::array<glm::vec3, 8> pos_vtx{
	glm::vec3(-0.5f,-0.5f,-0.5f),glm::vec3(0.5f,-0.5f,-0.5f),
		glm::vec3(0.5f,0.5f,-0.5f),glm::vec3(-0.5f,0.5f,-0.5f),
		glm::vec3(-0.5f,-0.5f,0.5f),glm::vec3(0.5f,-0.5f,0.5f),
		glm::vec3(0.5f,0.5f,0.5f),glm::vec3(-0.5f,0.5f,0.5f),
	};
	std::array<glm::vec3, 8> clr_vtx{
		glm::vec3(0.f, 1.f, 0.f), glm::vec3(1.f, 0.f, 0.f),
		glm::vec3(1.f, 1.f, 0.f),glm::vec3(0.f, 1.f, 0.f),
		glm::vec3(0.f, 0.f, 1.f),glm::vec3(1.f, 0.f, 1.f),
		glm::vec3(1.f, 1.f, 1.f),glm::vec3(0.f, 1.f, 1.f),
	};

	//pos_vtx[0] * rot;
	//for(int i = 0; i < 8; i++)
	//{
	//	pos_vtx[i] = rot * pos_vtx[i];
	//}
	GLuint vbo_hdl;
	glCreateBuffers(1, &vbo_hdl);



	glNamedBufferStorage(vbo_hdl,
		sizeof(glm::vec3) * pos_vtx.size() + sizeof(glm::vec3) * clr_vtx.size(),
		nullptr, GL_DYNAMIC_STORAGE_BIT);
	glNamedBufferSubData(vbo_hdl, 0, sizeof(glm::vec3) * pos_vtx.size(), pos_vtx.data());
	glNamedBufferSubData(vbo_hdl, sizeof(glm::vec3) * pos_vtx.size(), sizeof(glm::vec3) * clr_vtx.size(), clr_vtx.data());

	GLint max_vtx_attribs;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_vtx_attribs);
	std::cout << "Maximum vertex attributes: " << max_vtx_attribs << '\n';

	glCreateVertexArrays(1, &vaoid);
	glEnableVertexArrayAttrib(vaoid, 8);
	glVertexArrayVertexBuffer(vaoid, 3, vbo_hdl, 0, sizeof(glm::vec3));
	glVertexArrayAttribFormat(vaoid, 8, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vaoid, 8, 3);

	glEnableVertexArrayAttrib(vaoid, 9);
	glVertexArrayVertexBuffer(vaoid, 4, vbo_hdl, sizeof(glm::vec3) * pos_vtx.size(), sizeof(glm::vec3));
	glVertexArrayAttribFormat(vaoid, 9, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vaoid, 9, 4);

	primitive_type = GL_TRIANGLES;
	std::array<GLushort, 36> idx_vtx{
	0,3,3,3,1,2,
	0,1,4,4,5,1,
	1,2,5,5,6,2,
	2,3,6,6,7,3,
	3,7,4,4,3,0,
	4,5,7,7,6,5};
	idx_elem_cnt = static_cast<GLuint>(idx_vtx.size());
	GLuint ebo_hdl;
	glCreateBuffers(1, &ebo_hdl);
	glNamedBufferStorage(ebo_hdl, sizeof(GLushort) * idx_elem_cnt, reinterpret_cast<GLvoid*>(idx_vtx.data()), GL_DYNAMIC_STORAGE_BIT);
	glVertexArrayElementBuffer(vaoid, ebo_hdl);

	

	glBindVertexArray(0);

}

void GLApp::GLModel::setup_shdrpgm() {
	std::vector<std::pair<GLenum, std::string>> shdr_files;
	shdr_files.push_back(std::make_pair(GL_VERTEX_SHADER, "../shaders/fog.vert"));
	shdr_files.push_back(std::make_pair( GL_FRAGMENT_SHADER, "../shaders/fog.frag"));
	shdr_pgm.CompileLinkValidate(shdr_files);
	if (GL_FALSE == shdr_pgm.IsLinked()) {
		std::cout << "Unable to compile/link/validate shader programs" << "\n";
		std::cout << shdr_pgm.GetLog() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

void GLApp::GLModel::draw() {
	shdr_pgm.Use();

	glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	projection = glm::perspective(
		glm::radians(45.0f), 900.f / 900.f, 0.1f, 100.0f);


	GLint modelLoc = glGetUniformLocation(shdr_pgm.GetHandle(), "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	GLint viewLoc = glGetUniformLocation(shdr_pgm.GetHandle(), "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
	shdr_pgm.SetUniform("projection", projection);
	
	glBindVertexArray(vaoid);

	glDrawElements(primitive_type, idx_elem_cnt, GL_UNSIGNED_SHORT, NULL);

	glBindVertexArray(0);

	shdr_pgm.UnUse();
}
