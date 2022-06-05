// Dong-A Choi, Sunwoo Lee
// CS250 Class Project
// CS250
// 2022 spring
#include <Model_demo.h>
#include <IG.h>
/*                                                   objects with file scope
----------------------------------------------------------------------------- */
//static GLNew    g_glnew;

//ImVec4 clear_color = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);

void Models::init() {
	glClearColor(1.f, 1.f, 1.f, 1.f);
	glViewport(0, 0, GLHelper::width, GLHelper::height);

	camera = new Camera();
	camera->Set_eye(20);

	shdr_files.push_back(std::make_pair(GL_VERTEX_SHADER, "../shaders/model_shader.vert"));
	shdr_files.push_back(std::make_pair(GL_FRAGMENT_SHADER, "../shaders/model_shader.frag"));

	

	// shd, color, position, scale, rot(?)
	Plane.init(shdr_files, Vec4(0.1f, 0.1f, 0.9f, 1.00f), { -3,1.,-1 }, { 2,2,2 }, { 0,0,0 });

	Cube.init(shdr_files, Vec4(0.1f, 0.1f, 0.9f, 1.00f), { 0,1.,-1 }, { 2,2,2 }, { 0,0,0 });

	Sphere.init(shdr_files, Vec4(0.3f, 0.3f, 0.7f, 1.00f), { 3,1.,-1 }, { 2,2,2 }, { 0,0,0 });

	Cylinder.init(shdr_files, Vec4(0.1f, 0.1f, 0.9f, 1.00f), { -3,-1.,-1 }, { 2,2,2 }, { 0,0,0 });

	Cone.init(shdr_files, Vec4(0.7f, 0.7f, 1.0f, 1.0f), { 0,-1.,-1 }, { 2,2,2 }, { 0,0,0 });

	Torus.init(shdr_files, Vec4(1.0f, 0.0f, 0.0f, 1.0f), { 3,-1.,-1 }, { 2,2,2 }, { 0,0,0 });
}

void Models::update(double delta_time) {

	switch (static_cast<int>(GLHelper::currCameraMode))
	{
	case static_cast<int>(GLHelper::CameraMode::UP): camera->MoveUp(); GLHelper::currCameraMode = GLHelper::CameraMode::IDLE; break;
	case static_cast<int>(GLHelper::CameraMode::DOWN):camera->MoveDown(); GLHelper::currCameraMode = GLHelper::CameraMode::IDLE; break;
	case static_cast<int>(GLHelper::CameraMode::LEFT):camera->MoveLeft(); GLHelper::currCameraMode = GLHelper::CameraMode::IDLE; break;
	case static_cast<int>(GLHelper::CameraMode::RIGHT):camera->MoveRight(); GLHelper::currCameraMode = GLHelper::CameraMode::IDLE; break;
	case static_cast<int>(GLHelper::CameraMode::CLOSER):camera->MoveCloser(); GLHelper::currCameraMode = GLHelper::CameraMode::IDLE; break;
	case static_cast<int>(GLHelper::CameraMode::FARTHER):camera->MoveFarther(); GLHelper::currCameraMode = GLHelper::CameraMode::IDLE; break;
	}

	if (camera->eyeMoved || camera->resized)
	{
		Plane.compute_matrix(static_cast<float>(delta_time), camera->Get_eye(), camera->Get_frustum());
		Cube.compute_matrix(static_cast<float>(delta_time), camera->Get_eye(), camera->Get_frustum());
		Sphere.compute_matrix(static_cast<float>(delta_time), camera->Get_eye(), camera->Get_frustum());
		Cylinder.compute_matrix(static_cast<float>(delta_time), camera->Get_eye(), camera->Get_frustum());
		Cone.compute_matrix(static_cast<float>(delta_time), camera->Get_eye(), camera->Get_frustum());
		Torus.compute_matrix(static_cast<float>(delta_time), camera->Get_eye(), camera->Get_frustum());
	}

	ImGui::SliderFloat3("PlanePos", &Plane.Get_position().x, -15.f, 15.f);
	Plane.set_position(Plane.Get_position());
	ImGui::SliderFloat3("CubePose", &Cube.Get_position().x, -15.f, 15.f);
	Cube.set_position(Cube.Get_position());
	ImGui::SliderFloat3("SpherePos", &Sphere.Get_position().x, -15.f, 15.f);
	Sphere.set_position(Sphere.Get_position());
	ImGui::SliderFloat3("CylinderPos", &Cylinder.Get_position().x, -15.f, 15.f);
	Cylinder.set_position(Cylinder.Get_position());
	ImGui::SliderFloat3("ConePos", &Cone.Get_position().x, -15.f, 15.f);
	Cone.set_position(Cone.Get_position());
	ImGui::SliderFloat3("TorusPos", &Torus.Get_position().x, -15.f, 15.f);
	Torus.set_position(Torus.Get_position());

	if (ImGui::SliderInt2("Plane Slice&Stack", &PlaneSS.x, 5, 20)) 
	{
		Plane = CreatePlane(PlaneSS.x, PlaneSS.y);
		Plane.init(shdr_files, Vec4(0.1f, 0.1f, 0.9f, 1.00f), { -3,1.,-1 }, { 2,2,2 }, { 0,0,0 });
		Plane.compute_matrix(static_cast<float>(delta_time), camera->Get_eye(), camera->Get_frustum());
	}
	if (ImGui::SliderInt2("Cube Slice&Stack", &CubeSS.x, 5, 20))
	{
		Cube = CreateCube(CubeSS.x, CubeSS.y);
		Cube.init(shdr_files, Vec4(0.1f, 0.1f, 0.9f, 1.00f), { 0,1.,-1 }, { 2,2,2 }, { 0,0,0 });
		Cube.compute_matrix(static_cast<float>(delta_time), camera->Get_eye(), camera->Get_frustum());
	}
	if (ImGui::SliderInt2("Sphere Slice&Stack", &SphereSS.x, 8, 32))
	{
		Sphere = CreateSphere(SphereSS.x, SphereSS.y);
		Sphere.init(shdr_files, Vec4(0.3f, 0.3f, 0.7f, 1.00f), { 3,1.,-1 }, { 2,2,2 }, { 0,0,0 });
		Sphere.compute_matrix(static_cast<float>(delta_time), camera->Get_eye(), camera->Get_frustum());
	}
	if (ImGui::SliderInt2("Cylinder Slice&Stack", &CylinderSS.x, 1, 16))
	{
		Cylinder = CreateCylinder(CylinderSS.x, CylinderSS.y);
		Cylinder.init(shdr_files, Vec4(0.1f, 0.1f, 0.9f, 1.00f), { -3,-1.,-1 }, { 2,2,2 }, { 0,0,0 });
		Cylinder.compute_matrix(static_cast<float>(delta_time), camera->Get_eye(), camera->Get_frustum());
	}
	if (ImGui::SliderInt2("Cone Slice&Stack", &ConeSS.x, 4, 32))
	{
		Cone = CreateCone(ConeSS.x, ConeSS.y);
		Cone.init(shdr_files, Vec4(0.7f, 0.7f, 1.0f, 1.0f), { 0,-1.,-1 }, { 2,2,2 }, { 0,0,0 });
		Cone.compute_matrix(static_cast<float>(delta_time), camera->Get_eye(), camera->Get_frustum());
	}
	if (ImGui::SliderInt2("Torus Slice&Stack", &TorusSS.x, 8,32))
	{
		Torus = CreateTorus(TorusSS.x, TorusSS.y, 0, TWO_PI);
		Torus.init(shdr_files, Vec4(1.0f, 0.0f, 0.0f, 1.0f), { 3,-1.,-1 }, { 2,2,2 }, { 0,0,0 });
		Torus.compute_matrix(static_cast<float>(delta_time), camera->Get_eye(), camera->Get_frustum());
	}

	camera->eyeMoved = false;
	camera->resized = false;

	//mesh 각각 imgui 해서 포지션이나 로테이트 셋해서 메트릭스 업데이트 시키기
}

void Models::draw() {
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Plane.draw();
	Cube.draw();
	Sphere.draw();
	Cylinder.draw();
	Cone.draw();
	Torus.draw();
}

void Models::cleanup() {
	delete camera;
}


