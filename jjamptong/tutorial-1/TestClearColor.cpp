#include "TestClearColor.h"

#include <GL/glew.h>

namespace test
{
	TestClearColor::TestClearColor() : m_clearColor{0.2f,0.3f,0.8f,1.0f}
	{
		
	}

	TestClearColor::~TestClearColor()
	{
		
	}

	void TestClearColor::OnUpdate(float deltaTime)
	{
		
	}
	void TestClearColor::OnRender()
	{
		GLCall(glClearColor)
	}
	void TestClearColor::OnImGuiRender()
	{
		
	}



}
