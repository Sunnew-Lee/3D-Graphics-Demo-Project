#pragma once

#include <mesh.hpp>
#include <GLDemo.h>
#include <input.hpp>

class Tessellation : public GLDemo
{
public:
	Tessellation() :camera(nullptr) {};
	~Tessellation() override {};

	void init() override;
	void update(double delta_time) override;
	void draw() override;
	void cleanup() override;
private:

	Mesh plane = CreatePlane(7, 7);
	Camera* camera;
};