#include <GLDemo.h>
#include <glhelper.h>
#include <glslshader.h>
#include <array>

#include <IG.h>

/*                                                   objects with file scope
----------------------------------------------------------------------------- */
//static GLNew    g_glnew;
GLDemo::GLModel GLDemo::mdl;

void GLDemo::init() {
	glClearColor(1.f, 0.f, 0.f, 1.f);

	GLint w = GLHelper::width, h = GLHelper::height;
	glViewport(0, 0, w, h);

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

void GLDemo::update(double delta_time) {
	//glClearColor(0.f,0.f, 0.f, 1.f);
	IG::update();
}

void GLDemo::draw() {
	glClear(GL_COLOR_BUFFER_BIT);

	mdl.draw();
	IG::draw();
}

void GLDemo::cleanup() {
	// empty for now
	IG::cleanup();
}

void GLDemo::GLModel::setup_vao()
{
	std::array<glm::vec2, 3> pos_vtx{
	glm::vec2(-0.5f, -0.5f), glm::vec2(0.5f, -0.5f),
	glm::vec2(0.f, 0.5f)
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
	glBindVertexArray(0);

}

void GLDemo::GLModel::setup_shdrpgm() {
	std::vector<std::pair<GLenum, std::string>> shdr_files;
	shdr_files.push_back(std::make_pair(GL_VERTEX_SHADER, "../shaders/CS250_Project.vert"));
	shdr_files.push_back(std::make_pair(GL_FRAGMENT_SHADER, "../shaders/CS250_Project.frag"));
	shdr_pgm.CompileLinkValidate(shdr_files);
	if (GL_FALSE == shdr_pgm.IsLinked()) {
		std::cout << "Unable to compile/link/validate shader programs" << "\n";
		std::cout << shdr_pgm.GetLog() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

void GLDemo::GLModel::draw() {
	shdr_pgm.Use();

	glBindVertexArray(vaoid);

	glDrawElements(primitive_type, idx_elem_cnt, GL_UNSIGNED_SHORT, NULL);

	glBindVertexArray(0);
	shdr_pgm.UnUse();
}




