#pragma once
#include <GLDemo.h>

//class IG;

class P_Modeling : public GLDemo
{
public:
	P_Modeling() {};
	~P_Modeling() override {};

	void init() override;
	void update(double delta_time) override;
	void draw() override;
	void cleanup() override;
private:
	//IG* imgui;
};