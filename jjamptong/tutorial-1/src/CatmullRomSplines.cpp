#include <CatmullRomSplines.h>
#include <IG.h>
#include <vector>

void CatmullRomSplines::init() 
{
	P3Position = glm::vec2(0.5f, 0.0f);
	glViewport(0, 0, GLHelper::width, GLHelper::height);

	setup_shdrpgm();
	setup_vao();
	IG::init();
}

void CatmullRomSplines::update(double delta_time)
{
	IG::update();

	/*if (GLHelper::mouseClicked == true)
	{

		for (int i{ 0 }; i < curve_count; i++)
		{
			if (index >= 0 && index <= 13)
			{
				calc_vert(VERT_NUM);
			}
		}
		calc_vert();
		update_vao();
	}*/

	if (GLHelper::mouseClicked == true)
	{
		int size = pos_vtx.size();
		for (int i{ 0 }; i < size; i += VERT_NUM)
		{
			if (abs(GLHelper::mouse_pos.x - pos_vtx[i].x) < 0.05f && abs(GLHelper::mouse_pos.y - pos_vtx[i].y) < 0.05f)
			{
				is_clicked = true;
				index = i;
				break;
			}
			else if (abs(GLHelper::mouse_pos.x - pos_vtx[1 + i].x) < 0.05f && abs(GLHelper::mouse_pos.y - pos_vtx[1 + i].y) < 0.05f)
			{
				is_clicked = true;
				index = i + 1;
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
		for (int i{ 0 }; i < curve_count; i++)
		{
			if (index <= 1 || index >= 10)
			{
				//calc_vert(VERT_NUM);
				pos_vtx[index] = GLHelper::mouse_pos;
				calc_vert();
			}
		}
		update_vao();
	}
	
}

void CatmullRomSplines::draw()
{
	glClear(GL_COLOR_BUFFER_BIT);

	shdr_pgm.Use();
	glBindVertexArray(vaoid);

	glLineWidth(3.f);

	glVertexAttrib3f(9, 0.f, 0.f, 1.f); // blue color for lines
	glDrawArrays(GL_LINE_STRIP, 1, (VERT_NUM));

	//glVertexAttrib3f(9, 0.f, 1.f, 0.f); // green color for vectors
	//glDrawArrays(GL_LINE_STRIP, 0, 2);	//first two node
	//glDrawArrays(GL_LINE_STRIP, (VERT_NUM - 1), 2);//last two node

	glLineWidth(1.f);

	glPointSize(10.f);
	glVertexAttrib3f(9, 0.f, 1.f, 0.f); // green color for points
	glDrawArrays(GL_POINTS, 0, 1);
	glDrawArrays(GL_POINTS, 11, 1);

	glVertexAttrib3f(9, 1.f, 0.0f, 0.f); // red color for points
	glDrawArrays(GL_POINTS, 1, pos_vtx.size()-2);
	glPointSize(1.f);

	glBindVertexArray(0);

	shdr_pgm.UnUse();

	IG::draw();
}

void CatmullRomSplines::cleanup()
{
	IG::cleanup();
}

void CatmullRomSplines::setup_shdrpgm()
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

void CatmullRomSplines::setup_vao()
{
	glm::vec2 P0 = glm::vec2(-0.5f, -0.5f);
	glm::vec2 P1 = glm::vec2(-0.5f, 0.0f);	//start
	glm::vec2 P2 = P3Position;	//end
	glm::vec2 P3 = glm::vec2(0.5f, 0.5f);

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

void CatmullRomSplines::update_vao()
{

	glCreateVertexArrays(1, &vaoid);
	glGenBuffers(1, &vboid);

	glBindVertexArray(vaoid);
	glBindBuffer(GL_ARRAY_BUFFER, vboid);

	glBufferData(GL_ARRAY_BUFFER, pos_vtx.size() * sizeof(glm::vec2), pos_vtx.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(8);
	glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);

	glBindVertexArray(0);
}

//void CatmullRomSplines::vert_update(glm::vec2 P0, glm::vec2 P1, int count)
//{
//}
//	

void CatmullRomSplines::calc_vert()
{
	//VERT_NUM++;
	//P3Position = { GLHelper::world_mouse_pos.x,GLHelper::world_mouse_pos.y };
	glm::vec2 P1 = pos_vtx[1];
	glm::vec2 P2 = pos_vtx[10];
	
	for (int i{ 0 }; i <= VERT_NUM-1; i++)
	{
		float u = (1.f / (VERT_NUM - 1) * i);
		float u_cube = u * u * u;
		float u_square = u * u;

		glm::vec2 temp =
			(-1.f / 2.f * u_cube + u_square - 1.f / 2.f * u) * pos_vtx[0] + (3.f / 2.f * u_cube - 5.f / 2.f * u_square + 1.f) * P1
			+ (-3.f / 2.f * u_cube + 2.f * u_square + 1.f / 2.f * u) * P2 + (1.f / 2.f * u_cube - 1.f / 2.f * u_square) * pos_vtx[11];

		pos_vtx[i + 1] = temp;
	}
}
