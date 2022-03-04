// Dong-A Choi, Sunwoo Lee
// CS250 Class Project
// CS250
// 2022 spring
#include "glnew.h"

static void init();
static void draw();
static void update();
static void cleanup();

int main()
{
	
	init();
	update();
	draw();
	
	cleanup();

}

void init()
{
	GLNew::init();
}

void draw()
{
	GLNew::draw();
}

void update()
{
	GLNew::update();
}

void cleanup()
{
	GLNew::cleanup();
}
