#pragma once

struct Base
{
public:
	static void init();
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual void cleanup() = 0;

};