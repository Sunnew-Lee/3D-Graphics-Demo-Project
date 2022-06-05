// Dong-A Choi, Sunwoo Lee
// CS250 Class Project
// CS250
// 2022 spring
#include <Perlin_Terrain.h>
#include <IG.h>
/*                                                   objects with file scope
----------------------------------------------------------------------------- */
//static GLNew    g_glnew;



void Perlin_Terrain::init() {
	glClearColor(1.f, 1.f, 1.f, 1.f);
	glViewport(0, 0, GLHelper::width, GLHelper::height);

	camera = new Camera();
	camera->Set_eye(3);

	terrain = CreateTerrain(50, 50, frequency);
	
	shdr_files.push_back(std::make_pair(GL_VERTEX_SHADER, "../shaders/terrain.vert"));
	shdr_files.push_back(std::make_pair(GL_FRAGMENT_SHADER, "../shaders/terrain.frag"));

	terrain->init(shdr_files, Vec4(0.1f, 0.1f, 0.9f, 1.00f), { 0,0,0 }, { 3.,3.,1. }, { 0,0,0 });
	terrain->selfMat = Translate({ 0,0,0 }) * Rotate(QUARTER_PI, XAXIS)*  Scale({ 3.,3.,1. });
	terrain->compute_matrix(0.0, camera->Get_eye(), camera->Get_frustum());

}

void Perlin_Terrain::update(double delta_time) {

	terrain->UpdateTerrain(delta_time, frequency);
	terrain->UpdateVertexData();

	switch (static_cast<int>(GLHelper::currCameraMode))
	{
		case static_cast<int>(GLHelper::CameraMode::UP) : camera->MoveUp(); GLHelper::currCameraMode = GLHelper::CameraMode::IDLE; break;
		case static_cast<int>(GLHelper::CameraMode::DOWN) : camera->MoveDown(); GLHelper::currCameraMode = GLHelper::CameraMode::IDLE; break;
		case static_cast<int>(GLHelper::CameraMode::LEFT) : camera->MoveLeft(); GLHelper::currCameraMode = GLHelper::CameraMode::IDLE; break;
		case static_cast<int>(GLHelper::CameraMode::RIGHT) : camera->MoveRight(); GLHelper::currCameraMode = GLHelper::CameraMode::IDLE; break;
		case static_cast<int>(GLHelper::CameraMode::CLOSER) : camera->MoveCloser(); GLHelper::currCameraMode = GLHelper::CameraMode::IDLE; break;
		case static_cast<int>(GLHelper::CameraMode::FARTHER) : camera->MoveFarther(); GLHelper::currCameraMode = GLHelper::CameraMode::IDLE; break;
	}

	if (camera->eyeMoved || camera->resized)
	{
		terrain->compute_matrix(static_cast<float>(delta_time), camera->Get_eye(), camera->Get_frustum());
	}
	terrain->GetShdr_pgm().Use();
	glUniform3fv(terrain->lightPosLoc, 1, ValuePtr(terrain->lightPos));
	ImGui::SliderFloat3("LightPos", &terrain->lightPos.x, -10.f, 10.f);
	terrain->GetShdr_pgm().UnUse();
	if(ImGui::SliderInt2("Terrain Slice&Stack", &terrainSS.x,10,100))
	{
		terrain = CreateTerrain(terrainSS.x, terrainSS.y, frequency);
		terrain->init(shdr_files, Vec4(0.1f, 0.1f, 0.9f, 1.00f), { 0,0,0 }, { 3.,3.,1. }, { 0,0,0 });
		terrain->selfMat = Translate({ 0,0,0 }) * Rotate(QUARTER_PI, XAXIS) * Scale({ 3.,3.,1. });
		terrain->compute_matrix(0.0, camera->Get_eye(), camera->Get_frustum());
	}

	ImGui::SliderFloat("frequency", &frequency, 0.0f, 0.2f); 

	camera->eyeMoved = false;
	camera->resized = false;
}

void Perlin_Terrain::draw() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	terrain->draw();
}

void Perlin_Terrain::cleanup() {
	delete camera;
	delete terrain;
}


