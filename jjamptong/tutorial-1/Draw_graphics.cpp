#include <../Draw_graphics.h>
#include <glhelper.h>
#include <glslshader.h>
#include <array>
#include <../IG.h>
#include <glm/gtc/type_ptr.hpp>

// Dong-A Choi, Sunwoo Lee
// CS250 Class Project
// CS250
// 2022 spring
Draw_graphics::GLModel Draw_graphics::mdl;

void Draw_graphics::init() {
	glClearColor(1.f, 0.f, 0.f, 1.f);

	GLint w = GLHelper::width, h = GLHelper::height;
	glViewport(0, 0, w, h);

	mdl.setup_shdrpgm();
	mdl.setup_vao();

	IG::init();
}

void Draw_graphics::update(double delta_time) {
	//glClearColor(0.f,0.f, 0.f, 1.f);
	IG::update();
}

void Draw_graphics::draw() {
	glClear(GL_COLOR_BUFFER_BIT);

	mdl.draw();
	IG::draw();
}

void Draw_graphics::cleanup() {
	// empty for now
	IG::cleanup();
}

void Draw_graphics::GLModel::setup_vao()
{
	/*std::array<glm::vec3, 8> pos_vtx{
	glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f,-0.5f,-0.5f),
	glm::vec3(0.f, 0.5f)
	};
	std::array<glm::vec3, 3> clr_vtx{
	glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f),
	glm::vec3(0.f, 0.f, 1.f)
	};

	GLuint vbo_hdl;
	glCreateBuffers(1, &vbo_hdl);

	glNamedBufferStorage(vbo_hdl,
		sizeof(glm::vec2) * pos_vtx.size() + sizeof(glm::vec3) * clr_vtx.size(),
		nullptr, GL_DYNAMIC_STORAGE_BIT);
	glNamedBufferSubData(vbo_hdl, 0, sizeof(glm::vec2) * pos_vtx.size(), pos_vtx.data());
	glNamedBufferSubData(vbo_hdl, sizeof(glm::vec2) * pos_vtx.size(), sizeof(glm::vec3) * clr_vtx.size(), clr_vtx.data());

	GLint max_vtx_attribs;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_vtx_attribs);
	std::cout << "Maximum vertex attributes: " << max_vtx_attribs << '\n';

	glCreateVertexArrays(1, &vaoid);
	glEnableVertexArrayAttrib(vaoid, 8);
	glVertexArrayVertexBuffer(vaoid, 3, vbo_hdl, 0, sizeof(glm::vec2));
	glVertexArrayAttribFormat(vaoid, 8, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vaoid, 8, 3);

	glEnableVertexArrayAttrib(vaoid, 9);
	glVertexArrayVertexBuffer(vaoid, 4, vbo_hdl, sizeof(glm::vec2) * pos_vtx.size(), sizeof(glm::vec3));
	glVertexArrayAttribFormat(vaoid, 9, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vaoid, 9, 4);

	primitive_type = GL_TRIANGLES;
	std::array<GLushort, 3> idx_vtx{ 0, 1, 2 };
	idx_elem_cnt = static_cast<GLuint>(idx_vtx.size());
	GLuint ebo_hdl;
	glCreateBuffers(1, &ebo_hdl);
	glNamedBufferStorage(ebo_hdl, sizeof(GLushort) * idx_elem_cnt,
		reinterpret_cast<GLvoid*>(idx_vtx.data()), GL_DYNAMIC_STORAGE_BIT);
	glVertexArrayElementBuffer(vaoid, ebo_hdl);
	glBindVertexArray(0);*/

	MVpMatLoc = glGetUniformLocation(shdr_pgm.GetHandle(), "mvpMat");
	colorLoc = glGetUniformLocation(shdr_pgm.GetHandle(), "color");

	glGenVertexArrays(1, &mesh[0].VAO);
	glBindVertexArray(mesh[0].VAO);

	glGenBuffers(1, &mesh[0].VBO);
	glBindBuffer(GL_ARRAY_BUFFER, mesh[0].VBO);
	/*  Copy vertex attributes to GPU */
	glBufferData(GL_ARRAY_BUFFER,
		mesh[0].numVertices * vertexSize, &mesh[0].vertexBuffer[0],
		GL_STATIC_DRAW);

	glGenBuffers(1, &mesh[0].IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh[0].IBO);
	/*  Copy vertex indices to GPU */
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		mesh[0].numIndices * indexSize, &mesh[0].indexBuffer[0],
		GL_STATIC_DRAW);

	/*  Send vertex attributes to shaders */
	for (int i = 0; i < numAttribs; ++i)
	{
		glEnableVertexAttribArray(vLayout[i].location);
		glVertexAttribPointer(vLayout[i].location, vLayout[i].size, vLayout[i].type,
			vLayout[i].normalized, vertexSize, (void*)vLayout[i].offset);
	}
}

void Draw_graphics::GLModel::setup_shdrpgm() {
	std::vector<std::pair<GLenum, std::string>> shdr_files;
	shdr_files.push_back(std::make_pair(GL_VERTEX_SHADER, "../shaders/Draw_graphics.vert"));
	shdr_files.push_back(std::make_pair(GL_FRAGMENT_SHADER, "../shaders/Draw_graphics.frag"));
	shdr_pgm.CompileLinkValidate(shdr_files);
	if (GL_FALSE == shdr_pgm.IsLinked()) {
		std::cout << "Unable to compile/link/validate shader programs" << "\n";
		std::cout << shdr_pgm.GetLog() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

void Draw_graphics::GLModel::draw() {
	shdr_pgm.Use();

	glBindVertexArray(vaoid);

	glDrawElements(primitive_type, idx_elem_cnt, GL_UNSIGNED_SHORT, NULL);

	glBindVertexArray(0);
	shdr_pgm.UnUse();
}
