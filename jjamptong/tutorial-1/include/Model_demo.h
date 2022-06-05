// Dong-A Choi, Sunwoo Lee
// CS250 Class Project
// CS250
// 2022 spring
#pragma once
#include <glDemo.h>
#include <mesh.hpp>
#include <vector>
#include <input.hpp>

class Models :public GLDemo
{
public:
	Models():camera(nullptr){};
	~Models() override {};

	void init() override;
	void update(double delta_time) override;
	void draw() override;
	void cleanup() override;
private:
	glm::ivec2 PlaneSS{ 10,10 };
	glm::ivec2 CubeSS{ 10,10 };
	glm::ivec2 SphereSS{ 16,16};
	glm::ivec2 CylinderSS{ 1,8};
	glm::ivec2 ConeSS{ 16,8 };
	glm::ivec2 TorusSS{ 16,32 };

	Mesh Plane = CreatePlane(10, 10);
	Mesh Cube = CreateCube(10, 10);
	Mesh Sphere = CreateSphere(16, 16);
	Mesh Cylinder = CreateCylinder(1, 8);
	Mesh Cone = CreateCone(16, 8);
	Mesh Torus = CreateTorus(16, 32, 0, TWO_PI);
	
	Camera* camera;

	std::vector<std::pair<GLenum, std::string>> shdr_files;

};