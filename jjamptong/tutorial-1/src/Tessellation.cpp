#include <Tessellation.h>
#include <IG.h>
#include <graphics.hpp>
/*                                                   objects with file scope
----------------------------------------------------------------------------- */
//static GLNew    g_glnew;

void Tessellation::init() {
	glClearColor(1.f, 0.f, 0.f, 1.f);
	SetUp();

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

void Tessellation::update(double delta_time) {
	//glClearColor(0.f,0.f, 0.f, 1.f);

	glPatchParameteri(GL_PATCH_VERTICES, 3);

	IG::update();
	Render();
}

void Tessellation::draw() {
	//glClear(GL_COLOR_BUFFER_BIT);

	
	//mdl.draw();
	IG::draw();
}

void Tessellation::cleanup() {
	// empty for now
	IG::cleanup();
}


