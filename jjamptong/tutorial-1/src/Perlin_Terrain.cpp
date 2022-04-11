#include <Perlin_Terrain.h>
#include <IG.h>
#include <graphics.hpp>
/*                                                   objects with file scope
----------------------------------------------------------------------------- */
//static GLNew    g_glnew;

static float frequency = 64.f;

void Perlin_Terrain::init() {
	//glClearColor(1.f, 0.f, 0.f, 1.f);

	SetUp();	// set up the shaders and uniform

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

void Perlin_Terrain::update(double delta_time) {
	//glClearColor(0.f,0.f, 0.f, 1.f);
	Render(delta_time, frequency);	// update and draw terrain

	IG::update();
	ImGui::SliderFloat("frequency", &frequency, 2.0f, 64.0f);

}

void Perlin_Terrain::draw() {
	//glClear(GL_COLOR_BUFFER_BIT);

	//mdl.draw();
	IG::draw();
}

void Perlin_Terrain::cleanup() {
	// empty for now
	IG::cleanup();
}


