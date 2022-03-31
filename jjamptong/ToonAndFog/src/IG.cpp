#include <IG.h>

#include <glhelper.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


bool show_demo_window = false;
bool show_another_window = false;
ImVec4 clear_color = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);

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

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Test Menu");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color


        ImGui::End();
    }

    // 3. Show another simple window.
    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Choose another Demo!");

        if(ImGui::Button("RGB"))
        {
	        
        }
        if(ImGui::Button("White"))
        {
	        
        }
        if (ImGui::Button("Close Me")) {
            show_another_window = false;
        }
        ImGui::End();
    }
}

void IG::draw()
{
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void IG::cleanup()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
