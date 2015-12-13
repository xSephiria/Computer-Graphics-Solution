#include "Scene2.h"
#include "GL\glew.h"
#include "Application.h"

#include "shader.hpp"


Scene2::Scene2()
{
}

Scene2::~Scene2()
{
}

void Scene2::Init()
{
	// Init VBO here
	rotateAngle = 1;
	translateX = 1;
	scaleAll = 1;
	angle = 90;
	//Set background color to dark blue
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);


	//Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);


	//Generate buffers
	glGenBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glGenBuffers(NUM_GEOMETRY, &m_colorBuffer[0]);

	//Enable depth test
	glEnable(GL_DEPTH_TEST);


	//An array of 3 vectors which represents 3 vertices
	static const GLfloat vertex_buffer_data[] = {
		0.0f, 5.0f, 0.0f, //vertex 0 of triangle
		5.0f, -0.0f, 0.0f, //vertex 1 of triangle
		-5.0f, -0.0f, 0.0f, //vertex 2 of triangle
	};

	//An array of 3 vectors which represents the colors of the 3 vertices
	static const GLfloat color_buffer_data[] = {
		1.0f, 0.0f, 0.0f, //color of vertex 0
		0.0f, 1.0f, 0.0f, //color of vertex 1
		0.0f, 0.0f, 1.0f, //color of vertex 2
	};



	//Set the current active buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);
	//Transfer vertices to OpenGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data), color_buffer_data, GL_STATIC_DRAW);



	//Load vertex and fragment shaders
	m_programID = LoadShaders(
		"Shader//TransformVertexShader.vertexshader",
		"Shader//SimpleFragmentShader.fragmentshader"
		);

	//Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");

	//Use our Shader
	glUseProgram(m_programID);


}

void Scene2::Update(double dt)
{
	if (Application::IsKeyPressed(VK_SPACE))
	{
		rotateAngle += (float)(angle * dt);
		translateX += (float)(10 * dt);
		scaleAll += (float)(2 * dt);

		if (rotateAngle > 270 || rotateAngle < 0)
		{
			angle = -1 * angle;
		}
		if (scaleAll > 4)
		{
			scaleAll = 0;
		}
		if (translateX > 20)
		{
			translateX = -20;
		}
	}
}

void Scene2::Render()
{
	// Render VBO here

	Mtx44 translate, rotate, scale;
	Mtx44 model;
	Mtx44 view;
	Mtx44 projection;
	Mtx44 MVP;

	translate.SetToIdentity();
	rotate.SetToIdentity();
	scale.SetToIdentity();
	model.SetToIdentity();
	view.SetToIdentity(); //no need for camera, set to World's origin
	projection.SetToOrtho(-40, 40, -30, 30, -10, 10); // World is a cube defined by these boundaries
	//               ( -X,  X,  -Y,  Y,  -Z,  Z)

	// 1st triangle
	scale.SetToScale(2, 2, 2);
	rotate.SetToRotation(rotateAngle, 0, 0, 1);
	translate.SetToTranslation(0, 0, 0);
	model = translate * rotate * scale; // Scale then rotate then translate(Reverse order)
	MVP = projection * view * model; //Reverse order
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]); // Update shader with new MVP

	//Clear color & depth buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// First attribute buffer : vertices
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(
		0, // For attribute 0, must match the layout in the shader, usually 0 is for vertex
		3, // Size
		GL_FLOAT, // type
		GL_FALSE, // normalised?
		0, // stride
		0 // array buffer offset
		);

	// 2nd attribute buffer: colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0, 3 vertices = 1 triangle





	// 2nd triangle
	scale.SetToScale(1, 1, 1);
	rotate.SetToRotation(135, 0, 0, 1);
	translate.SetToTranslation(translateX, 15, 0);
	model = translate * rotate * scale; // Scale then rotate then translate(Reverse order)
	MVP = projection * view * model; //Reverse order
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]); // Update shader with new MVP

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);





	// 3rd triangle
	scale.SetToScale(scaleAll, scaleAll, scaleAll);
	rotate.SetToRotation(0, 0, 0, 1);
	translate.SetToTranslation(-20, -15, 0);
	model = translate * rotate * scale; // Scale then rotate then translate(Reverse order)
	MVP = projection * view * model; //Reverse order
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]); // Update shader with new MVP

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

}

void Scene2::Exit()
{
	// Cleanup VBO here
	glDeleteBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glDeleteBuffers(NUM_GEOMETRY, &m_colorBuffer[0]);
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
