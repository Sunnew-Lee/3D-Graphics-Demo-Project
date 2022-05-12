#pragma once
#include <GLDemo.h>

#include "glslshader.h"

//input = whole primitive type not just one vertex
//output = set of primitives
class Geometry : public GLDemo
{
public:
	Geometry() {};
	~Geometry() override {};

	void init() override;
	void update(double delta_time) override;
	void draw() override;
	void cleanup() override;

	GLSLShader getShader() { return shdr_pgm; }

private:

	void setup_shdrpgm();
	//void setup_vao();
	//void update_vao();

	//void setUpGeometry(GLSLShader shdr);
	//void drawGeometry();

	GLenum primitive_type{ 0 };
	GLSLShader shdr_pgm;

	glm::mat4 viewMat{ 0 };
	glm::mat4 projMat{ 0 };
	/*glm::mat4 geoMat {0 };*/

	const GLfloat one = 1.0f;
	//GLuint vaoid{ 0 };
	//GLuint idx_elem_cnt{ 0 };
};
