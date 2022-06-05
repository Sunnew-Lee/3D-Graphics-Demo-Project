#include <Tessellation.h>

/*                                                   objects with file scope
----------------------------------------------------------------------------- */
//static GLNew    g_glnew;

void Tessellation::init() {
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glViewport(0, 0, GLHelper::width, GLHelper::height);

	camera = new Camera();
	camera->Set_eye(3);

	std::vector<std::pair<GLenum, std::string>> shdr_files;
	shdr_files.push_back(std::make_pair(GL_VERTEX_SHADER, "../shaders/shader.vert"));
	shdr_files.push_back(std::make_pair(GL_GEOMETRY_SHADER, "../shaders/shader.geom"));
	shdr_files.push_back(std::make_pair(GL_FRAGMENT_SHADER, "../shaders/shader.frag"));
	shdr_files.push_back(std::make_pair(GL_TESS_CONTROL_SHADER, "../shaders/shader.tcs"));
	shdr_files.push_back(std::make_pair(GL_TESS_EVALUATION_SHADER, "../shaders/shader.tes"));

	// shd, color, position, scale, rot(?)
	plane.init(shdr_files, Vec4(0.1f, 0.1f, 0.9f, 1.00f), { 0,0,-2 }, { 3,3,1 }, { 0,0,0 });
	plane.selfMat = { Translate({0,0,0}) * Rotate(-HALF_PI, XAXIS) * Scale({ 30, 30, 1 }) };
}

void Tessellation::update(double delta_time) {
	glPatchParameteri(GL_PATCH_VERTICES, 3);

	switch (static_cast<int>(GLHelper::currCameraMode))
	{
		case static_cast<int>(GLHelper::CameraMode::UP) : camera->MoveUp(); GLHelper::currCameraMode = GLHelper::CameraMode::IDLE; break;
		case static_cast<int>(GLHelper::CameraMode::DOWN) : camera->MoveDown(); GLHelper::currCameraMode = GLHelper::CameraMode::IDLE; break;
		case static_cast<int>(GLHelper::CameraMode::LEFT) : camera->MoveLeft(); GLHelper::currCameraMode = GLHelper::CameraMode::IDLE; break;
		case static_cast<int>(GLHelper::CameraMode::RIGHT) : camera->MoveRight(); GLHelper::currCameraMode = GLHelper::CameraMode::IDLE; break;
		case static_cast<int>(GLHelper::CameraMode::CLOSER) : camera->MoveCloser(); GLHelper::currCameraMode = GLHelper::CameraMode::IDLE; break;
		case static_cast<int>(GLHelper::CameraMode::FARTHER) : camera->MoveFarther(); GLHelper::currCameraMode = GLHelper::CameraMode::IDLE; break;
	}

	plane.update_tess(camera->eyeRadius);
	if (camera->eyeMoved || camera->resized)
	{
		plane.compute_matrix(static_cast<float>(delta_time), camera->Get_eye(), camera->Get_frustum());
	}

	camera->eyeMoved = false;
	camera->resized = false;
}

void Tessellation::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	plane.draw();
}

void Tessellation::cleanup() {
	// empty for now
}


