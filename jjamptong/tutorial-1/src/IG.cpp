// Dong-A Choi, Sunwoo Lee
// CS250 Class Project
// CS250
// 2022 spring
#include <IG.h>

#include <glhelper.h>



void IG::init()
{
	// Setup ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// Setup Platform/Renderer bindings
	const char* glsl_version = "#version 330";
	ImGui_ImplOpenGL3_Init(glsl_version);
	ImGui_ImplGlfw_InitForOpenGL(GLHelper::ptr_window, true);
	ImGui::StyleColorsDark();
}

void IG::update()
{
    // ¿¹½Ã Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();


	ImGui::Begin("Class Project");

	

	//if (ImGui::BeginMenu("Credit"))
	//{
	//	if (ImGui::MenuItem("Close"))
	//	{
	//		ImGui::EndMenu();
	//	}
	//	else
	//	{
	//		ImGui::Text("Team: jjamptong");
	//		ImGui::Text("Members: sunwoo.lee / dong-a.choi");
	//	}

	//}
	ImGui::Text("Team: jjamptong");
	ImGui::Text("Members: sunwoo.lee / dong-a.choi");

	ImGui::NewLine();
	float framerate = ImGui::GetIO().Framerate;
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.f / framerate, framerate);

	ImGui::NewLine();
	ImGui::ColorEdit3("background color", (float*)&clear_color); // Edit 3 floats representing a color
	ImGui::Checkbox("Togle WireFrame", (bool*)&is_wireframe);

	if(is_wireframe == true)
	{
		GLHelper::currRenderMode = GLHelper::RenderMode::WIREFRAME;
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_CULL_FACE);

	}
	else
	{
		GLHelper::currRenderMode = GLHelper::RenderMode::NORMAL;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_CULL_FACE);
	}

	ImGui::NewLine();
	if(ImGui::Combo("Demos", &selectedItem, items, IM_ARRAYSIZE(items)))
	{
		switch (selectedItem)
		{
		case 0:curr_demo = DEMOS::MODEL; break;
		case 1:curr_demo = DEMOS::TOON; break;
		case 2:curr_demo = DEMOS::VALUE; break;
		case 3:curr_demo = DEMOS::PERLIN; break;
		case 4:curr_demo = DEMOS::TERRAIN; break;
		case 5:curr_demo = DEMOS::HERMITE; break;
		case 6:curr_demo = DEMOS::CATMULL; break;
		//case 7:curr_demo = DEMOS::GEOM; break;
		//case 8:curr_demo = DEMOS::SHADOW; break;

		default: curr_demo = DEMOS::MODEL; break;
		}
	}

	ImGui::End();
}

void IG::draw()
{
    
    
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void IG::cleanup()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void IG::Set_state(DEMOS next_demo)
{
    curr_demo = next_demo;
}

IG::DEMOS IG::Get_state()
{
    return curr_demo;
}
