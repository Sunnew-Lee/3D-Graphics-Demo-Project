#pragma once
#include <GLDemo.h>

//class IG;

class Perlin_Terrain : public GLDemo
{
public:
	Perlin_Terrain() {};
	~Perlin_Terrain() override {};

	void init() override;
	void update(double delta_time) override;
	void draw() override;
	void cleanup() override;
private:
	//IG* imgui;
};