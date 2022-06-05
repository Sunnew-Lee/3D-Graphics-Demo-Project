// Dong-A Choi, Sunwoo Lee
// CS250 Class Project
// CS250
// 2022 spring
#pragma once

#include <GLDemo.h>

//class IG;

class Tessellation : public GLDemo
{
public:
	Tessellation() {};
	~Tessellation() override {};

	void init() override;
	void update(double delta_time) override;
	void draw() override;
	void cleanup() override;
private:
	//IG* imgui;
};