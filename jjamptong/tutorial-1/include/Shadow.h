#pragma once

#include <GLDemo.h>

//class IG;

class Shadow : public GLDemo
{
public:
	Shadow() {};
	~Shadow() override {};

	void init() override;
	void update(double delta_time) override;
	void draw() override;
	void cleanup() override;
private:
	//IG* imgui;
};