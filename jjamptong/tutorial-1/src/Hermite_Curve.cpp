#include <Hermite_Curve.h>
#include <IG.h>
#include <vector>

void Hermite_Curve::init()
{
	glViewport(0, 0, GLHelper::width, GLHelper::height);

	setup_shdrpgm();
	setup_vao();
	IG::init();
}

void Hermite_Curve::update(double delta_time)
{
	IG::update();

	if (GLHelper::mouseClicked == true)
	{
		int size = pos_vtx.size();
		for (int i{ 0 }; i < size; i += VERT_NUM + 1)
		{
			if (abs(GLHelper::mouse_pos.x - pos_vtx[i].x) < 0.05f && abs(GLHelper::mouse_pos.y - pos_vtx[i].y) < 0.05f)
			{
				is_clicked = true;
				index = i;
				break;
			}
			else if (abs(GLHelper::mouse_pos.x - pos_vtx[1+i].x) < 0.05f && abs(GLHelper::mouse_pos.y - pos_vtx[1+i].y) < 0.05f)
			{
				is_clicked = true;
				index = i + 1;
				break;
			}
		}
	}
	if (is_clicked == true)
	{
		pos_vtx[index] = GLHelper::mouse_pos;
		if (GLHelper::mouseClicked == false)
		{
			is_clicked = false;
		}
		for (int i{ 0 }; i < curve_count; i++)
		{
			if (index >= 0 && index <= 13)
			{
				calc_vert(VERT_NUM);
			}
		}
		update_vao();
	}

	//if (ImGui::Button("New Line"))
	//{
	//	curve_count++;
	//	glm::vec2 temp = *(pos_vtx.end() - 1);
	//	calc_vert(*(pos_vtx.end() - 2), *(pos_vtx.end() - 1), VERT_NUM);
	//	update_vao();
	//}
}

void Hermite_Curve::draw()
{
	glClear(GL_COLOR_BUFFER_BIT);

	shdr_pgm.Use();
	glBindVertexArray(vaoid);

	//for (int i{ 0 }; i < curve_count; i++)
	//{
	//	glLineWidth(3.f);

	//	glVertexAttrib3f(9, 0.f, 0.f, 1.f); // blue color for lines
	//	glDrawArrays(GL_LINE_STRIP, (VERT_NUM + 3) * i + 1, (VERT_NUM + 1));

	//	glVertexAttrib3f(9, 0.f, 1.f, 0.f); // green color for vectors
	//	glDrawArrays(GL_LINE_STRIP, (VERT_NUM + 3) * i, 2);	//pos_vtx[0],pos_vtx[1]
	//	glDrawArrays(GL_LINE_STRIP, (VERT_NUM + 3) * i + (VERT_NUM + 1), 2);	//pos_vtx[11],pos_vtx[12]

	//	glLineWidth(1.f);
	//}

	glLineWidth(3.f);

	glVertexAttrib3f(9, 0.f, 0.f, 1.f); // blue color for lines
	glDrawArrays(GL_LINE_STRIP, 1, (VERT_NUM + 1));

	glVertexAttrib3f(9, 0.f, 1.f, 0.f); // green color for vectors
	glDrawArrays(GL_LINE_STRIP, 0, 2);	//pos_vtx[0],pos_vtx[1]
	glDrawArrays(GL_LINE_STRIP, (VERT_NUM + 1), 2);	//pos_vtx[11],pos_vtx[12]

	glLineWidth(1.f);

	glPointSize(10.f);
	glVertexAttrib3f(9, 1.f, 0.0f, 0.f); // red color for points
	glDrawArrays(GL_POINTS, 0, pos_vtx.size());
	glPointSize(1.f);

	glBindVertexArray(0);

	shdr_pgm.UnUse();

	IG::draw();
}

void Hermite_Curve::cleanup()
{
	IG::cleanup();
}

void Hermite_Curve::setup_shdrpgm()
{
	std::vector<std::pair<GLenum, std::string>> shdr_files;
	shdr_files.push_back(std::make_pair(GL_VERTEX_SHADER, "../shaders/CS250_Project.vert"));
	shdr_files.push_back(std::make_pair(GL_FRAGMENT_SHADER, "../shaders/CS250_Project.frag"));
	shdr_pgm.CompileLinkValidate(shdr_files);
	if (GL_FALSE == shdr_pgm.IsLinked()) {
		std::cout << "Unable to compile/link/validate shader programs" << "\n";
		std::cout << shdr_pgm.GetLog() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

void Hermite_Curve::setup_vao()
{
	glm::vec2 P0 = glm::vec2(-0.5f, 0.0f);
	glm::vec2 P1 = glm::vec2(0.5f, 0.0f);
	glm::vec2 P0_p = glm::vec2(P0.x, -0.75f);
	glm::vec2 P1_p = glm::vec2(P1.x, -0.75f);

	pos_vtx.push_back(P0_p);

	for (int i{ 0 }; i <= VERT_NUM; i++)
	{
		float u = (1.f / VERT_NUM * i);
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
	//glm::vec2 P0 = glm::vec2(-0.5f, 0.0f);
	//glm::vec2 P1 = glm::vec2(0.5f, 0.0f);

	//calc_vert(pos_vtx[1], pos_vtx[11], VERT_NUM);

	glBindVertexArray(vaoid);
	glBindBuffer(GL_ARRAY_BUFFER, vboid);

	glBufferData(GL_ARRAY_BUFFER, pos_vtx.size() * sizeof(glm::vec2), pos_vtx.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(8);
	glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);

	glBindVertexArray(0);
}

void Hermite_Curve::vert_update(glm::vec2 P0, glm::vec2 P1, int count)
{
	//glm::vec2 temp = *(pos_vtx.end() - 2);
	//glm::vec2 P1 = glm::vec2(temp.x + 0.25f, temp.y);
	//glm::vec2 P1_p = glm::vec2(P1.x, -0.75f);

	//pos_vtx.push_back(P0_p);

	//for (int i{ 0 }; i <= count; i++)
	//{
	//	float u = (1.f / count * i);
	//	float u_cube = u * u * u;
	//	float u_square = u * u;

	//	glm::vec2 temp =
	//		(2 * u_cube - 3 * u_square + 1) * P0 + (-2 * u_cube + 3 * u_square) * P1
	//		+ (u_cube - 2 * u_square + u) * (P0_p - P0) + (u_cube - u_square) * (P1_p - P1);

	//	pos_vtx.push_back(temp);
	//}
	//pos_vtx.push_back(P1_p);
}

void Hermite_Curve::calc_vert( int count)
{
	//glm::vec2 temp = *(pos_vtx.end() - 2);
	//glm::vec2 P1 = glm::vec2(temp.x + 0.25f, temp.y);
	//glm::vec2 P1_p = glm::vec2(P1.x, -0.75f);

	for (int i{ 0 }; i <= count; i++)
	{
		float u = (1.f / count * i);
		float u_cube = u * u * u;
		float u_square = u * u;

		glm::vec2 temp =
			(2 * u_cube - 3 * u_square + 1) * pos_vtx[1] + (-2 * u_cube + 3 * u_square) * pos_vtx[11]
			+ (u_cube - 2 * u_square + u) * (pos_vtx[0] - pos_vtx[1]) + (u_cube - u_square) * (pos_vtx[12] - pos_vtx[11]);

		pos_vtx[i + 1] = temp;
	}
}
