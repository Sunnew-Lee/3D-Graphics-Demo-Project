// Dong-A Choi, Sunwoo Lee
// CS250 Class Project
// CS250
// 2022 spring
#pragma once
#include <glslshader.h>
#include <GLDemo.h>
#include <vector>

class Hermite_Curve : public GLDemo
{
public:
	Hermite_Curve() {};
	~Hermite_Curve() override {};

	void init() override;
	void update(double delta_time) override;
	void draw() override;
	void cleanup() override;
private:
	void setup_shdrpgm();
	void setup_vao();
	void update_vao();
	void add_vao();
	GLuint vaoid = 0;
	GLuint vboid = 0;

	GLSLShader shdr_pgm;
	std::vector<glm::vec2> pos_vtx;
	std::vector<glm::vec2> moveable_vrx{
		glm::vec2(-0.5f, -0.75f),glm::vec2(-0.5f, 0.0f),glm::vec2(0.5f, 0.0f),glm::vec2(0.5f, -0.75f),glm::vec2(0.5f, 0.0f)
	};

	int curve_count = 1;
	bool is_clicked = false;
	int VERT_NUM = 4;
	int index{ 0 };
	bool hide_tangent = false;
};