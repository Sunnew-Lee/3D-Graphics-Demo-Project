#include <Geometry.h>
#include <IG.h>
#include <graphics.hpp>

void Geometry::init()
{
	glViewport(0, 0, GLHelper::width, GLHelper::height);

	setup_shdrpgm();
	setUpGeometry(getShader());

	//setup_vao();
	IG::init();
}

void Geometry::update(double/* delta_time*/)
{
	
	IG::update();
}

void Geometry::draw()
{
	//drawGeometry();
	RenderGeo(getShader());
	IG::draw();

}

void Geometry::cleanup()
{
	IG::cleanup();
}


void Geometry::setup_shdrpgm()
{
	std::vector<std::pair<GLenum, std::string>> shdr_files;
	shdr_files.push_back(std::make_pair(GL_VERTEX_SHADER, "../shaders/Geometry.vert"));
	shdr_files.push_back(std::make_pair(GL_FRAGMENT_SHADER, "../shaders/Geometry.frag"));
	shdr_files.push_back(std::make_pair(GL_GEOMETRY_SHADER, "../shaders/Geometry.gs"));
	shdr_pgm.CompileLinkValidate(shdr_files);
	if (GL_FALSE == shdr_pgm.IsLinked()) {
		std::cout << "Unable to compile/link/validate shader programs" << "\n";
		std::cout << shdr_pgm.GetLog() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

//void Geometry::setup_vao()
//{
//	
//}
//
//void Geometry::update_vao()
//{
//	
//}

//void Geometry::setUpGeometry(GLSLShader shdr)
//{
//	shdr.Use();
//
//	/*  Obtain the locations of the variables in the shaders with the given names */
//	GLint viewMatLoc = glGetUniformLocation(shdr.GetHandle(), "View");
//	GLint ProjMatLoc = glGetUniformLocation(shdr.GetHandle(), "Projection");
//	GLint GeoMatLoc = glGetUniformLocation(shdr.GetHandle(), "GeometryTransform");
//	viewMat = glm::translate(viewMat, glm::vec3(0.0f, 0.0f, -3.0f));
//	projMat = glm::perspective(glm::radians(45.0f), 
//		(float)GLHelper::width / (float)GLHelper::height, 0.1f, 100.0f);
//	geoMat = { {1,0,0,0},
//					{0,1,0,0},
//					{0,0,1,0},
//					{0,0,0,1} };
//
//	glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, glm::value_ptr(viewMat));
//	glUniformMatrix4fv(ProjMatLoc, 1, GL_FALSE, glm::value_ptr(projMat));
//	glUniformMatrix4fv(GeoMatLoc, 1, GL_FALSE, glm::value_ptr(geoMat));
//
//	SendVertexData(mesh[i]);
//
//	/*  Bind framebuffer to 0 to render to the screen (by default already 0) */
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//	/*  Initially drawing using filled mode */
//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//
//	glEnable(GL_CULL_FACE);     /*  For efficiency, not drawing back-face */
//
//	shdr.UnUse();
//}

//void Geometry::drawGeometry()
//{
//	shdr_pgm.Use();
//	glClearBufferfv(GL_DEPTH, 0, &one);
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	UpdateTransform();
//	UpdateUniforms_Draw(part, partMVPMat);
//
//
//	shdr_pgm.UnUse();
//}
