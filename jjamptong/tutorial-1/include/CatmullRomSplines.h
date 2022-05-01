#pragma once
#include <glslshader.h>
#include <GLDemo.h>
#include <vector>

//class IG;

class CatmullRomSplines : public GLDemo
{
public:
	CatmullRomSplines() {};
	~CatmullRomSplines() override {};

	void init() override;
	void update(double delta_time) override;
	void draw() override;
	void cleanup() override;
private:
	void setup_shdrpgm();
	void setup_vao();
	void update_vao();
	GLuint vaoid;
	GLuint vboid;

	GLSLShader shdr_pgm;
	std::vector<glm::vec2> pos_vtx;
	//void vert_update(glm::vec2 P0, glm::vec2 P1, int count);
	void calc_vert();
	int curve_count = 2;
	bool is_clicked = false;
	int VERT_NUM = 3;
	int index{ 0 };

	glm::vec2 P3Position;
	//IG* imgui;
};