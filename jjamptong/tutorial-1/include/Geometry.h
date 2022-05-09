#pragma once

#include <GLDemo.h>

//class IG;

class Geometry : public GLDemo
{
public:
	Geometry() {};
	~Geometry() override {};

	void init() override;
	void update(double delta_time) override;
	void draw() override;
	void cleanup() override;
private:
	//IG* imgui;
};