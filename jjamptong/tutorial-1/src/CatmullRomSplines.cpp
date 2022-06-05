// Dong-A Choi, Sunwoo Lee
// CS250 Class Project
// CS250
// 2022 spring
#include <CatmullRomSplines.h>

void CatmullRomSplines::init() 
{
	glViewport(0, 0, GLHelper::width, GLHelper::height);

	setup_shdrpgm();
	setup_vao();
}

void CatmullRomSplines::update([[maybe_unused]] double delta_time)
{
	if (ImGui::SliderInt("addVertex", &VERT_NUM, 4, 12))
	{
		pos_vtx.clear();
		pos_vtx.resize((VERT_NUM + 2) * curve_count);
		update_vao();
	}

	if (ImGui::Button("addLine"))
	{
		moveable_vrx.push_back({ 0,0 });
		add_vao();
	}

	if (ImGui::Button("Clear"))
	{
		cleanup();
		moveable_vrx.clear();
		moveable_vrx.push_back(glm::vec2(-0.5f, -0.5f));
		moveable_vrx.push_back(glm::vec2(-0.5f, 0.0f));
		moveable_vrx.push_back(glm::vec2(0.5f, 0.0f));
		moveable_vrx.push_back(glm::vec2(0.5f, 0.5f));
		pos_vtx.clear();
		VERT_NUM = 4;
		curve_count = 1;

		setup_vao();
	}

	if (GLHelper::mouseClicked == GL_TRUE)
	{
		int size = static_cast<int>(moveable_vrx.size());
		for (int i{ 0 }; i < size; i++)
		{
			if (abs(GLHelper::mouse_pos.x - moveable_vrx[i].x) < 0.05f && abs(GLHelper::mouse_pos.y - moveable_vrx[i].y) < 0.05f)
			{
				is_clicked = true;
				index = i;
				break;
			}
		}
	}
	if (is_clicked == true)
	{
		
		if (GLHelper::mouseClicked == false)
		{
			is_clicked = false;
		}
		moveable_vrx[index] = GLHelper::mouse_pos;
		update_vao();
	}
	
}

void CatmullRomSplines::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shdr_pgm.Use();
	glBindVertexArray(vaoid);

	glLineWidth(3.f);

	glVertexAttrib3f(9, 0.f, 0.f, 1.f); // blue color for lines

	int size_in_one_curve = VERT_NUM + 2;

	for (int i{ 0 }; i < curve_count; i++)
	{
		glDrawArrays(GL_LINE_STRIP, 1 + size_in_one_curve * i, (VERT_NUM));
	}

	glLineWidth(1.f);

	glPointSize(10.f);
	glVertexAttrib3f(9, 0.f, 1.f, 0.f); // green color for points

	for (int i{ 0 }; i < curve_count; i++)
	{
		glDrawArrays(GL_POINTS, size_in_one_curve * i, 2);
		glDrawArrays(GL_POINTS, VERT_NUM + (size_in_one_curve * i), 2);
	}

	glPointSize(1.f);

	glBindVertexArray(0);

	shdr_pgm.UnUse();
}

void CatmullRomSplines::cleanup()
{
}

void CatmullRomSplines::setup_shdrpgm()
{
	std::vector<std::pair<GLenum, std::string>> shdr_files;
	shdr_files.push_back(std::make_pair(GL_VERTEX_SHADER, "../shaders/curve.vert"));
	shdr_files.push_back(std::make_pair(GL_FRAGMENT_SHADER, "../shaders/curve.frag"));
	shdr_pgm.CompileLinkValidate(shdr_files);
	if (GL_FALSE == shdr_pgm.IsLinked()) {
		std::cout << "Unable to compile/link/validate shader programs" << "\n";
		std::cout << shdr_pgm.GetLog() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

void CatmullRomSplines::setup_vao()
{
	glm::vec2 P0 = moveable_vrx[0];
	glm::vec2 P1 = moveable_vrx[1];
	glm::vec2 P2 = moveable_vrx[2];
	glm::vec2 P3 = moveable_vrx[3];

	pos_vtx.push_back(P0);

	for (int i{ 0 }; i <= VERT_NUM-1; i++)
	{
		float u = (1.f / (VERT_NUM-1) * i);
		float u_cube = u * u * u;
		float u_square = u * u;

		glm::vec2 temp =
			( -1.f/2.f * u_cube + u_square - 1.f/2.f * u) * P0 + (3.f/2.f * u_cube -5.f/2.f * u_square + 1.f) * P1
			+ (-3.f/2.f * u_cube + 2.f * u_square + 1.f/2.f * u) * P2 + (1.f/2.f * u_cube - 1.f/2.f * u_square) *P3;

		pos_vtx.push_back(temp);
	}

	pos_vtx.push_back(P3);

	glCreateVertexArrays(1, &vaoid);
	glGenBuffers(1, &vboid);

	glBindVertexArray(vaoid);
	glBindBuffer(GL_ARRAY_BUFFER, vboid);

	glBufferData(GL_ARRAY_BUFFER, pos_vtx.size() * sizeof(glm::vec2), pos_vtx.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(8);
	glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);

	glBindVertexArray(0);

}

void CatmullRomSplines::add_vao()
{
	glm::vec2 P0 = moveable_vrx[curve_count];
	glm::vec2 P1 = moveable_vrx[curve_count + 1];
	glm::vec2 P2 = moveable_vrx[curve_count + 2];
	glm::vec2 P3 = moveable_vrx[curve_count + 3];
	curve_count++;

	pos_vtx.push_back(P0);

	for (int i{ 0 }; i <= VERT_NUM - 1; i++)
	{
		float u = (1.f / (VERT_NUM - 1) * i);
		float u_cube = u * u * u;
		float u_square = u * u;

		glm::vec2 temp =
			(-1.f / 2.f * u_cube + u_square - 1.f / 2.f * u) * P0 + (3.f / 2.f * u_cube - 5.f / 2.f * u_square + 1.f) * P1
			+ (-3.f / 2.f * u_cube + 2.f * u_square + 1.f / 2.f * u) * P2 + (1.f / 2.f * u_cube - 1.f / 2.f * u_square) * P3;

		pos_vtx.push_back(temp);
	}

	pos_vtx.push_back(P3);
	
	glBindVertexArray(vaoid);
	glBindBuffer(GL_ARRAY_BUFFER, vboid);

	glBufferData(GL_ARRAY_BUFFER, pos_vtx.size() * sizeof(glm::vec2), pos_vtx.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(8);
	glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);

	glBindVertexArray(0);
}

void CatmullRomSplines::update_vao()
{
	for (int j{ 0 }; j < curve_count; j++)
	{
		glm::vec2 P0 = moveable_vrx[j];
		glm::vec2 P1 = moveable_vrx[j + 1];
		glm::vec2 P2 = moveable_vrx[j + 2];
		glm::vec2 P3 = moveable_vrx[j + 3];

		int stride = (VERT_NUM + 2) * j;

		pos_vtx[stride] = P0;
		pos_vtx[VERT_NUM + 1 + stride] = P3;

		for (int i{ 0 }; i <= VERT_NUM - 1; i++)
		{
			float u = (1.f / (VERT_NUM - 1) * i);
			float u_cube = u * u * u;
			float u_square = u * u;
			

			glm::vec2 temp =
				(-1.f / 2.f * u_cube + u_square - 1.f / 2.f * u) * pos_vtx[stride] + (3.f / 2.f * u_cube - 5.f / 2.f * u_square + 1.f) * P1
				+ (-3.f / 2.f * u_cube + 2.f * u_square + 1.f / 2.f * u) * P2 + (1.f / 2.f * u_cube - 1.f / 2.f * u_square) * pos_vtx[VERT_NUM + 1 + stride];

			pos_vtx[(i + 1) + stride] = temp;
		}
	}

	glBindVertexArray(vaoid);
	glBindBuffer(GL_ARRAY_BUFFER, vboid);

	glBufferData(GL_ARRAY_BUFFER, pos_vtx.size() * sizeof(glm::vec2), pos_vtx.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(8);
	glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);

	glBindVertexArray(0);
}