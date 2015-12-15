#include "Assignment2.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"
#include "Utility.h"
#include "Application.h"
#include "MeshBuilder.h"

Assignment2::Assignment2()
{
}

Assignment2::~Assignment2()
{
}

void Assignment2::Init()
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


	m_programID = LoadShaders("Shader//Shading.vertexshader",
		"Shader//Shading.fragmentshader");
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	glUseProgram(m_programID);

	light[0].position.Set(0, 20, 0);
	light[0].color.Set(1, 1, 1);
	light[0].power = 1;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;

	// Make sure you pass uniform parameters after glUseProgram()
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);



	//variable to rotate geometry

	rotateBlade = 0.f;
	translateModel = 0.f;
	//Initialize camera settings
	camera.Init(Vector3(50, 40, 40), Vector3(0, 0, 0), Vector3(0, 1, 0));

	//axes, floor and light source
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1));
	meshList[GEO_QUAD]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_QUAD]->material.kDiffuse.Set(0.9f, 0.9f, 0.9f);
	meshList[GEO_QUAD]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_QUAD]->material.kShininess = 1.f;

	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 1, 1), 10, 20);

	//pb render
	meshList[GEO_PINKBEANBODY] = MeshBuilder::GenerateCylinder("PBBody", Color(1.f, 0.000f, 1.f), 36);
	meshList[GEO_PINKBEANBODY]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_PINKBEANBODY]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_PINKBEANBODY]->material.kSpecular.Set(0.7f, 0.7f, 0.7f);

	meshList[GEO_PINKBEANEAR] = MeshBuilder::GenerateCone("PBEar", Color(0.580f, 0.000f, 0.827f), 36, 2, 4);
	meshList[GEO_PINKBEANEAR]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_PINKBEANEAR]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_PINKBEANEAR]->material.kSpecular.Set(0.7f, 0.7f, 0.7f);

	meshList[GEO_PINKBEANHEAD] = MeshBuilder::GenerateSphere("PBHead", Color(1.f, 0.000f, 1.f), 36, 36);
	meshList[GEO_PINKBEANHEAD]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_PINKBEANHEAD]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_PINKBEANHEAD]->material.kSpecular.Set(0.7f, 0.7f, 0.7f);

	meshList[GEO_PINKBEANLOWERBODY] = MeshBuilder::GenerateSphere("PBLowerBody", Color(1.f, 0.000f, 1.f), 36, 36);
	meshList[GEO_PINKBEANLOWERBODY]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_PINKBEANLOWERBODY]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_PINKBEANLOWERBODY]->material.kSpecular.Set(0.7f, 0.7f, 0.7f);

	meshList[GE0_PINKBEANINNEREYE] = MeshBuilder::GenerateSphere("innereye", Color(0.941f, 0.973f, 1.000f), 36, 36);
	meshList[GE0_PINKBEANINNEREYE]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GE0_PINKBEANINNEREYE]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GE0_PINKBEANINNEREYE]->material.kSpecular.Set(0.7f, 0.7f, 0.7f);

	meshList[GEO_PINKBEANOUTEREYE] = MeshBuilder::GenerateSphere("outereye", Color(0.f, 0.f, 0.f), 36, 36);
	meshList[GEO_PINKBEANOUTEREYE]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_PINKBEANOUTEREYE]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_PINKBEANOUTEREYE]->material.kSpecular.Set(0.7f, 0.7f, 0.7f);

	meshList[GEO_PINKBEANEYEMARK] = MeshBuilder::GenerateCube("eyemark", Color(0.941f, 0.973f, 1.000f));
	meshList[GEO_PINKBEANEYEMARK]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_PINKBEANEYEMARK]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_PINKBEANEYEMARK]->material.kSpecular.Set(0.7f, 0.7f, 0.7f);

	meshList[GEO_PINKBEANBELLY] = MeshBuilder::GenerateCylinder("Belly", Color(0.941f, 0.973f, 1.000f), 36);
	meshList[GEO_PINKBEANBELLY]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_PINKBEANBELLY]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_PINKBEANBELLY]->material.kSpecular.Set(0.7f, 0.7f, 0.7f);

	meshList[GEO_PINKBEANARMS] = MeshBuilder::GenerateSphere("Arm", Color(1.000f, 0.078f, 0.576f), 36, 36);
	meshList[GEO_PINKBEANARMS]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_PINKBEANARMS]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_PINKBEANARMS]->material.kSpecular.Set(0.7f, 0.7f, 0.7f);

	meshList[GEO_PINKBEANLEGS] = MeshBuilder::GenerateSphere("Legs", Color(0.863f, 0.078f, 0.235f), 36, 36);
	meshList[GEO_PINKBEANLEGS]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_PINKBEANLEGS]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_PINKBEANLEGS]->material.kSpecular.Set(0.7f, 0.7f, 0.7f);

	// item
	meshList[GEO_PROPELLERBASE] = MeshBuilder::GenerateHemiSphere("PropellerBase", Color(1.000f, 0.647f, 0.000f), 36, 36);
	meshList[GEO_PROPELLERBASE]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_PROPELLERBASE]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_PROPELLERBASE]->material.kSpecular.Set(0.7f, 0.7f, 0.7f);

	meshList[GEO_PROPELLERROD] = MeshBuilder::GenerateCylinder("PropellerRod", Color(1.000f, 0.843f, 0.000f), 36);
	meshList[GEO_PROPELLERROD]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_PROPELLERROD]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_PROPELLERROD]->material.kSpecular.Set(0.7f, 0.7f, 0.7f);

	meshList[GEO_PROPELLERBLADESCONNECTOR] = MeshBuilder::GenerateSphere("PropellerBladeConnector", Color(1.000f, 0.647f, 0.000f), 36, 36);
	meshList[GEO_PROPELLERBLADESCONNECTOR]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_PROPELLERBLADESCONNECTOR]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_PROPELLERBLADESCONNECTOR]->material.kSpecular.Set(0.7f, 0.7f, 0.7f);

	meshList[GEO_PROPELLERBLADES] = MeshBuilder::GenerateSphere("PropellerBlade", Color(1.000f, 0.647f, 0.000f), 36, 36);
	meshList[GEO_PROPELLERBLADES]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_PROPELLERBLADES]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_PROPELLERBLADES]->material.kSpecular.Set(0.7f, 0.7f, 0.7f);

	Mtx44 projection; 
	projection.SetToPerspective(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
	projectionStack.LoadMatrix(projection);
}

static float ROT_LIMIT = 45.f;
static float SCALE_LIMIT = 5.f;
static float LSPEED = 10.f;

void Assignment2::Update(double dt)
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

	if (Application::IsKeyPressed('I'))
		light[0].position.z -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('K'))
		light[0].position.z += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('J'))
		light[0].position.x -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('L'))
		light[0].position.x += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('O'))
		light[0].position.y -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('P'))
		light[0].position.y += (float)(LSPEED * dt);

	rotateBlade += (10 * dt);

	if (Application::IsKeyPressed(VK_SPACE))
	{
		rotateBlade += 30;
	}
	else
	{
		rotateBlade += (10 * dt);
		//translateModel += (10 * dt);
	}
}

void Assignment2::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	MVP = projectionStack.Top() * viewStack.Top() *	modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
	if (enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);
		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	mesh->Render();
}

void Assignment2::Render()
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


	Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
	glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);

	//Axes
	RenderMesh(meshList[GEO_AXES], false);

	//Light Source
	modelStack.PushMatrix();
	modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	//Floor
	modelStack.PushMatrix();
	modelStack.Scale(100, 100, 100);
	modelStack.Translate(0, -0.13f, 0);
	RenderMesh(meshList[GEO_QUAD], true);
	modelStack.PopMatrix();
	
	// Whole model
	modelStack.PushMatrix();
	//modelStack.Translate(0, translateModel, 0);
	//Pb
		modelStack.PushMatrix();

		modelStack.PushMatrix();
			modelStack.Translate(-4, 12, 0);
			modelStack.Rotate(45.f, 0, 0, 1);
			RenderMesh(meshList[GEO_PINKBEANEAR], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(4, 12, 0);
		modelStack.Rotate(-45.f, 0, 0, 1);
		RenderMesh(meshList[GEO_PINKBEANEAR], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
			modelStack.Scale(5.3f, 15, 5.3f);
			RenderMesh(meshList[GEO_PINKBEANBODY], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
			modelStack.Scale(5, 12, 5);
			modelStack.Translate(0, 0, 0.1f);
			RenderMesh(meshList[GEO_PINKBEANBELLY], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
			modelStack.Translate(-6, 0, 0);
			modelStack.Rotate(90.f, 0, 0, 1);
			modelStack.Scale(4, 0.8f, 0.8f);
			RenderMesh(meshList[GEO_PINKBEANARMS], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
			modelStack.Translate(6, 0, 0);
			modelStack.Rotate(90.f, 0, 0, 1);
			modelStack.Scale(4, 0.8f, 0.8f);
			RenderMesh(meshList[GEO_PINKBEANARMS], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
			modelStack.Scale(5.3f, 5.3f, 5.3f);
			modelStack.Translate(0, 1.4f, 0);
			modelStack.Rotate(180.f, 1, 0, 0);
			RenderMesh(meshList[GEO_PINKBEANHEAD], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Scale(0.5f, 0.5f, 0.1f);
		modelStack.Translate(-4.f, 20, 40.f);
		modelStack.Rotate(-45.f, 0, 1, 0);
		RenderMesh(meshList[GE0_PINKBEANINNEREYE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
			modelStack.Scale(5.3f, 5.3f, 5.3f);
			modelStack.Translate(0, -1.4f, 0);
			modelStack.Rotate(180.f, 1, 0, 0);
			RenderMesh(meshList[GEO_PINKBEANLOWERBODY], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
			modelStack.Translate(3.5, -12, 0);
			modelStack.Rotate(90.f, 0, 1, 0);
			modelStack.Rotate(35.f, 1, 0, 0);
			modelStack.Rotate(30.f, 0, 1, 0);
			modelStack.Scale(4, 1, 1);
			RenderMesh(meshList[GEO_PINKBEANLEGS], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
			modelStack.Translate(-3.5, -12, 0);
			modelStack.Rotate(90.f, 0, 1, 0);
			modelStack.Rotate(-35.f, 1, 0, 0);
			modelStack.Rotate(-30.f, 0, 1, 0);
			modelStack.Scale(4, 1, 1);
			RenderMesh(meshList[GEO_PINKBEANLEGS], true);
		modelStack.PopMatrix();

		modelStack.PopMatrix();

		//propeller
		modelStack.PushMatrix();

			modelStack.Translate(0, 12, 0);
			modelStack.PushMatrix();
			modelStack.Scale(1.5, 1.5, 1.5);
			modelStack.Rotate(180, 0, 0, 1);
			RenderMesh(meshList[GEO_PROPELLERBASE], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Scale(0.2, 2.5, 0.2);
			modelStack.Translate(0, 1, 0);
			RenderMesh(meshList[GEO_PROPELLERROD], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();

			modelStack.Rotate(rotateBlade, 0, 1, 0);

				modelStack.PushMatrix();
				modelStack.Scale(0.3, 0.3, 0.3);
				modelStack.Translate(0, 13, 0);
				RenderMesh(meshList[GEO_PROPELLERBLADESCONNECTOR], true);
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Scale(3, 0.2f, 1);
				modelStack.Translate(1, 20, 0);
				RenderMesh(meshList[GEO_PROPELLERBLADES], true);
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Scale(3, 0.2f, 1);
				modelStack.Translate(-1, 20, 0);
				RenderMesh(meshList[GEO_PROPELLERBLADES], true);
				modelStack.PopMatrix();

			modelStack.PopMatrix();
	
		modelStack.PopMatrix();

	modelStack.PopMatrix();
}

void Assignment2::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}