// Dong-A Choi, Sunwoo Lee
// CS250 Class Project
// CS250
// 2022 spring
#include <Hermite_Curve.h>

void Hermite_Curve::init()
{
	glViewport(0, 0, GLHelper::width, GLHelper::height);

	setup_shdrpgm();
	setup_vao();
}

void Hermite_Curve::update([[maybe_unused]]double delta_time)
{
	if (ImGui::SliderInt("addVertex", &VERT_NUM, 4, 12))
	{
		pos_vtx.clear();
		pos_vtx.resize((VERT_NUM + 2) * curve_count);
		update_vao();
	}

	ImGui::Checkbox("Hide Tangent", (bool*)&hide_tangent);

	if (ImGui::Button("addLine"))
	{
		moveable_vrx.push_back({ 0,0 });	//end point
		moveable_vrx.push_back({ 0,-0.5 });	//end tangent
		moveable_vrx.push_back({ 0,0 });	//end point
		add_vao();
	}

	if (ImGui::Button("Clear"))
	{
		cleanup();
		moveable_vrx.clear();
		moveable_vrx.push_back(glm::vec2(-0.5f, -0.75f));
		moveable_vrx.push_back(glm::vec2(-0.5f, 0.0f));
		moveable_vrx.push_back(glm::vec2(0.5f, 0.0f));
		moveable_vrx.push_back(glm::vec2(0.5f, -0.75f));
		moveable_vrx.push_back(glm::vec2(0.5f, 0.0f));
		pos_vtx.clear();
		VERT_NUM = 4;
		curve_count = 1;
		hide_tangent = false;

		setup_vao();
	}

	if (GLHelper::mouseClicked == GL_TRUE && is_clicked == false)
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

		if (index < moveable_vrx.size() - 2)
		{
			if(moveable_vrx[index] == moveable_vrx[index+2])
				moveable_vrx[index+2] = GLHelper::mouse_pos;
		}
		moveable_vrx[index] = GLHelper::mouse_pos;

		update_vao();
	}

}

void Hermite_Curve::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shdr_pgm.Use();
	glBindVertexArray(vaoid);

	glLineWidth(3.f);

	glVertexAttrib3f(9, 0.f, 0.f, 1.f); // blue color for lines

	int size_in_one_curve = VERT_NUM + 2;

	for (int i{ 0 }; i < curve_count; i++)
	{
		glDrawArrays(GL_LINE_STRIP, 1 + size_in_one_curve*i, (VERT_NUM));
	}

	glVertexAttrib3f(9, 0.f, 1.f, 0.f); // green color for vectors

	if (hide_tangent == false)
	{
		for (int i{ 0 }; i < curve_count; i++)
		{
			glDrawArrays(GL_LINE_STRIP, size_in_one_curve * i, 2);	//pos_vtx[0],pos_vtx[1]
			glDrawArrays(GL_LINE_STRIP, VERT_NUM + (size_in_one_curve * i), 2);	//pos_vtx[11],pos_vtx[12]
		}
	}


	glLineWidth(1.f);

	glPointSize(10.f);
	glVertexAttrib3f(9, 1.f, 0.0f, 0.f); // red color for points

	for (int i{ 0 }; i < curve_count; i++)
	{
		glDrawArrays(GL_POINTS, size_in_one_curve * i, 2);	//pos_vtx[0],pos_vtx[1]
		glDrawArrays(GL_POINTS, VERT_NUM + (size_in_one_curve * i), 2);	//pos_vtx[11],pos_vtx[12]
	}

	glPointSize(1.f);

	glBindVertexArray(0);

	shdr_pgm.UnUse();

}

void Hermite_Curve::cleanup()
{
}

void Hermite_Curve::setup_shdrpgm()
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

void Hermite_Curve::setup_vao()
{
	glm::vec2 P0_p = moveable_vrx[0];	//P0 tangent
	glm::vec2 P0 = moveable_vrx[1];
	glm::vec2 P1 = moveable_vrx[2];
	glm::vec2 P1_p = moveable_vrx[3];	//P1 tangent

	pos_vtx.push_back(P0_p);

	for (int i{ 0 }; i <= VERT_NUM-1; i++)
	{
		float u = (1.f / (VERT_NUM-1) * i);
		float u_cube = u * u * u;
		float u_square = u * u;

		glm::vec2 temp =
			(2 * u_cube - 3 * u_square + 1) * P0 + (-2 * u_cube + 3 * u_square) * P1
			+ (u_cube - 2 * u_square + u) * (P0_p - P0) + (u_cube - u_square) * (P1_p - P1);

		pos_vtx.push_back(temp);
	}
	pos_vtx.push_back(P1_p);

	glCreateVertexArrays(1, &vaoid);
	glGenBuffers(1, &vboid);

	glBindVertexArray(vaoid);
	glBindBuffer(GL_ARRAY_BUFFER, vboid);

	glBufferData(GL_ARRAY_BUFFER, pos_vtx.size() * sizeof(glm::vec2), pos_vtx.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(8);
	glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);

	glBindVertexArray(0);

}

void Hermite_Curve::update_vao()
{
	for (int j{ 0 }; j < curve_count; j++)
	{
		glm::vec2 P0_p = moveable_vrx[j * 3];
		glm::vec2 P0 = moveable_vrx[j * 3 + 1];
		glm::vec2 P1 = moveable_vrx[j * 3 + 2];
		glm::vec2 P1_p = moveable_vrx[j * 3 + 3];

		int stride = (VERT_NUM + 2) * j;

		pos_vtx[stride] = P0_p;
		pos_vtx[VERT_NUM + 1 + stride] = P1_p;

		for (int i{ 0 }; i <= VERT_NUM - 1; i++)
		{
			float u = (1.f / (VERT_NUM - 1) * i);
			float u_cube = u * u * u;
			float u_square = u * u;

			glm::vec2 temp =
				(2 * u_cube - 3 * u_square + 1) * P0 + (-2 * u_cube + 3 * u_square) * P1
				+ (u_cube - 2 * u_square + u) * (P0_p - P0) + (u_cube - u_square) * (P1_p - P1);

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

void Hermite_Curve::add_vao()
{
	glm::vec2 P0_p = moveable_vrx[curve_count * 3];	//P0 tangent
	glm::vec2 P0 = moveable_vrx[curve_count * 3 +1];
	glm::vec2 P1 = moveable_vrx[curve_count * 3 +2];
	glm::vec2 P1_p = moveable_vrx[curve_count * 3 +3];	//P1 tangent
	curve_count++;

	pos_vtx.push_back(P0_p);

	for (int i{ 0 }; i <= VERT_NUM - 1; i++)
	{
		float u = (1.f / (VERT_NUM - 1) * i);
		float u_cube = u * u * u;
		float u_square = u * u;

		glm::vec2 temp =
			(2 * u_cube - 3 * u_square + 1) * P0 + (-2 * u_cube + 3 * u_square) * P1
			+ (u_cube - 2 * u_square + u) * (P0_p - P0) + (u_cube - u_square) * (P1_p - P1);

		pos_vtx.push_back(temp);
	}
	pos_vtx.push_back(P1_p);

	glBindVertexArray(vaoid);
	glBindBuffer(GL_ARRAY_BUFFER, vboid);

	glBufferData(GL_ARRAY_BUFFER, pos_vtx.size() * sizeof(glm::vec2), pos_vtx.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(8);
	glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);

	glBindVertexArray(0);
}
