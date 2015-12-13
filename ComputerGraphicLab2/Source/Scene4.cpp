#include "Scene4.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"

#include "Application.h"
#include "MeshBuilder.h"

scene4::scene4()
{
}

scene4::~scene4()
{
}

void scene4::Init()
{
	// Init VBO here

	// Set background color to dark blue
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	//Enable depth buffer and depth testing
	glEnable(GL_DEPTH_TEST);

	//Enable back face culling
	glEnable(GL_CULL_FACE);

	//Default to fill mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	//Load vertex and fragment shaders
	m_programID = LoadShaders("Shader//TransformVertexShader.vertexshader", "Shader//SimpleFragmentShader.fragmentshader");
	// Use our shader
	glUseProgram(m_programID);
	// Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");

	//variable to rotate geometry
	rotateAngle = 0;
	planet1RotAngle = planet1RevAngle = moon1RotAngle = 0;
	StarRot = MercuryRot = VenusRot = EarthRot = MoonRot = MarsRot = JupiterRot = SaturnRot = SatRingRot = UranusRot = NeptuneRot = PlutoRot = 0;
	//Initialize camera settings
	camera.Init(Vector3(50, 40, 40), Vector3(0, 0, 0), Vector3(0, 1, 0));

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(1, 1, 0));
	meshList[GEO_CIRCLE] = MeshBuilder::GenerateCircle("circle", Color(1, 0, 1), 20);
	meshList[GEO_RING] = MeshBuilder::GenerateRing("ring", Color(0, 1, 1), 0.8f, 6);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1, 1, 1), 10, 20);
	
	meshList[GEO_STAR] = MeshBuilder::GenerateSphere("star", Color(1, 1, 0), 10, 20);
	meshList[GEO_MERCURY] = MeshBuilder::GenerateSphere("mercury", Color(.545f, .271f, .075f), 10, 20);
	meshList[GEO_VENUS] = MeshBuilder::GenerateSphere("venus", Color(.855f, .647f, .125f), 10, 20);
	meshList[GEO_EARTH] = MeshBuilder::GenerateSphere("earth", Color(.196f, .804f, .196f), 10, 20);
	meshList[GEO_MOON] = MeshBuilder::GenerateSphere("moon", Color(.8f, 1, .3f), 10, 20);
	meshList[GEO_MARS] = MeshBuilder::GenerateSphere("mars", Color(.698f, .133f, .133f), 10, 20);
	meshList[GEO_JUPITER] = MeshBuilder::GenerateSphere("jupiter", Color(1, .871f, .678f), 10, 20);
	meshList[GEO_SATURN] = MeshBuilder::GenerateSphere("saturn", Color(1, .9f, .8f), 10, 20);
	meshList[GEO_SATRING] = MeshBuilder::GenerateRing("satring", Color(1, .9f, .6f), 0.8f, 20);
	meshList[GEO_URANUS] = MeshBuilder::GenerateSphere("uranus", Color(.69f, 0.878f, .902f), 10, 20);
	meshList[GEO_NEPTUNE] = MeshBuilder::GenerateSphere("neptune", Color(.69f, .769f, .871f), 10, 20);
	meshList[GEO_PLUTO] = MeshBuilder::GenerateSphere("pluto", Color(.878f, 1, 1), 10, 20);
	
	Mtx44 projection;
	projection.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
	projectionStack.LoadMatrix(projection);
}

static float ROT_LIMIT = 45.f;
static float SCALE_LIMIT = 5.f;

void scene4::Update(double dt)
{
	camera.Update(dt);

	if (Application::IsKeyPressed('1')) //enable back face culling
		glEnable(GL_CULL_FACE);
	if (Application::IsKeyPressed('2')) //disable back face culling
		glDisable(GL_CULL_FACE);
	if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode

	rotateAngle += (float)(10 * dt);
	planet1RotAngle += (float)(5 * dt);
	planet1RevAngle += (float)(2 * dt);
	moon1RotAngle += (float)(50 * dt);
	StarRot += (float)(3 * dt);
	MercuryRot += (float)(12 * dt);
	VenusRot += (float)(11 * dt);
	EarthRot += (float)(10 * dt);
	MoonRot += (float)(9 * dt);
	MarsRot += (float)(8 * dt);
	JupiterRot += (float)(7 * dt);
	SaturnRot += (float)(6 * dt);
	SatRingRot += (float)(5 * dt);
	UranusRot += (float)(4 * dt);
	NeptuneRot += (float)(3 * dt);
	PlutoRot += (float)(2 * dt);
}

void scene4::Render()
{
	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Set view matrix using camera settings
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
		);

	modelStack.LoadIdentity();

	Mtx44 MVP;

	modelStack.PushMatrix();
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_AXES]->Render();
	modelStack.PopMatrix();

	modelStack.PushMatrix();						//planet1 system

		modelStack.Rotate(StarRot, 0, 1, 0);	//revolution about centre


		modelStack.PushMatrix();
		modelStack.Scale(12, 12, 12);
		MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		meshList[GEO_STAR]->Render();
		modelStack.PopMatrix();

	modelStack.PopMatrix();
	modelStack.PushMatrix();

		modelStack.Translate(0, 0, 0);
		modelStack.Rotate(MercuryRot, 0, 1, 0);
		modelStack.Translate(15, 0, 0);

		modelStack.PushMatrix();
		MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		meshList[GEO_MERCURY]->Render();
		modelStack.PopMatrix();

	modelStack.PopMatrix();
	modelStack.PushMatrix();

		modelStack.Translate(0, 0, 0);
		modelStack.Rotate(VenusRot, 0, 1, 0);
		modelStack.Translate(20, 0, 0);

		modelStack.PushMatrix();
		modelStack.Scale(3, 3, 3);
		MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		meshList[GEO_VENUS]->Render();
		modelStack.PopMatrix();

	modelStack.PopMatrix();
	modelStack.PushMatrix();

	modelStack.Translate(0, 0, 0);
	modelStack.Rotate(EarthRot, 0, 1, 0);
	modelStack.Translate(29, 0, 0);					//offset from centre

		modelStack.PushMatrix();						//moon
		modelStack.Rotate(moon1RotAngle, 1, 0, 0);	//revolution about planet
		modelStack.Translate(0, 10, 0);					//offset from planet
		MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		meshList[GEO_MOON]->Render();
		modelStack.PopMatrix();

		modelStack.PushMatrix();						//planet1 itself
		modelStack.Rotate(planet1RotAngle, 0, 1, 0);	//rotation about its own axis
			modelStack.PushMatrix();
			modelStack.Scale(5, 5, 5);						//planet size
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[GEO_EARTH]->Render();
			modelStack.PopMatrix();
		modelStack.PopMatrix();

	modelStack.PopMatrix();
	modelStack.PushMatrix();

		modelStack.Translate(0, 0, 0);
		modelStack.Rotate(MarsRot, 0, 1, 0);
		modelStack.Translate(38, 0, 0);
	
		modelStack.PushMatrix();
		modelStack.Scale(3, 3, 3);
		MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		meshList[GEO_MARS]->Render();
		modelStack.PopMatrix();

	modelStack.PopMatrix();
	modelStack.PushMatrix();

		modelStack.Translate(0, 0, 0);
		modelStack.Rotate(JupiterRot, 0, 1, 0);
		modelStack.Translate(49, 0, 0);

		modelStack.PushMatrix();
		modelStack.Scale(7, 7, 7);
		MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		meshList[GEO_JUPITER]->Render();
		modelStack.PopMatrix();

	modelStack.PopMatrix();
	modelStack.PushMatrix();

		modelStack.Translate(0, 0, 0);
		modelStack.Rotate(SaturnRot, 0, 1, 0);
		modelStack.Translate(64, 0, 0);

		modelStack.PushMatrix();
		modelStack.Scale(6, 6, 6);
		MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		meshList[GEO_SATURN]->Render();
		modelStack.PopMatrix();

		modelStack.PushMatrix();						//Ring
		modelStack.Rotate(45, 0, 0, 1);
		modelStack.Scale(8, 8, 8);
		MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		meshList[GEO_SATRING]->Render();
		modelStack.PopMatrix();

	modelStack.PopMatrix();
	modelStack.PushMatrix();

		modelStack.Translate(0, 0, 0);
		modelStack.Rotate(UranusRot, 0, 1, 0);
		modelStack.Translate(75, 0, 0);

		modelStack.PushMatrix();
		modelStack.Scale(4, 4, 4);
		MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		meshList[GEO_URANUS]->Render();
		modelStack.PopMatrix();

	modelStack.PopMatrix();
	modelStack.PushMatrix();

		modelStack.Translate(0, 0, 0);
		modelStack.Rotate(NeptuneRot, 0, 1, 0);
		modelStack.Translate(84, 0, 0);

		modelStack.PushMatrix();
		modelStack.Scale(3.5f, 3.5f, 3.5f);
		MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		meshList[GEO_NEPTUNE]->Render();
		modelStack.PopMatrix();

	modelStack.PopMatrix();
	modelStack.PushMatrix();

		modelStack.Translate(0, 0, 0);
		modelStack.Rotate(PlutoRot, 0, 1, 0);
		modelStack.Translate(89, 0, 0);

		modelStack.PushMatrix();
		modelStack.Scale(1, 1, 1);
		MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		meshList[GEO_PLUTO]->Render();
		modelStack.PopMatrix();



	modelStack.PopMatrix();
}

void scene4::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}