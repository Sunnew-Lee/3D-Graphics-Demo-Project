#pragma once

#include <GLDemo.h>

//class IG;

class Toon_Fog : public GLDemo
{
public:
	Toon_Fog() {};
	~Toon_Fog() override {};

	void init() override;
	void update(double delta_time) override;
	void draw() override;
	void cleanup() override;
private:
	//IG* imgui;
};