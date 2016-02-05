#include "Assignment3.h"
#include "GL\glew.h"
#include "LoadTGA.h"
#include "LoadOBJ.h"
#include "shader.hpp"
#include "Mtx44.h"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "Application.h"

Assignment3::Assignment3()
{
}

Assignment3::~Assignment3()
{
}

void Assignment3::Init()
{
	// Init VBO here

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Set background color
	glEnable(GL_DEPTH_TEST); // Enable depth buffer and depth testing
	glEnable(GL_CULL_FACE); // Enable back face culling
	glEnable(GL_BLEND); //Enable Blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Default to fill mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");

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
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights"); //in case you missed out practical 7
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textColor");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	glUseProgram(m_programID);

	//variable to rotate geometry

	//Initialize camera settings
	camera.Init(Vector3(2, 1, 0), Vector3(0, 0, 0), Vector3(0, 1, 0));

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("AXES", 1000, 1000, 1000);

	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);


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
	glUniform1i(m_parameters[U_NUMLIGHTS], 1);

	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("LIGHT", Color(1, 1, 1), 36, 36);

	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1));
	meshList[GEO_QUAD]->textureID = LoadTGA("Image//color2.tga");

	//Front
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1));
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//blizzardfront.tga");

	//Back
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1));
	meshList[GEO_BACK]->textureID = LoadTGA("Image//blizzardback.tga");

	//Left
	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1));
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//blizzardright.tga");

	//Right
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1));
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//blizzardleft.tga");

	//Top
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1));
	meshList[GEO_TOP]->textureID = LoadTGA("Image//blizzardtop.tga");

	//Bottom
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1));
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//blizzardbottom.tga");

	//Wall
	meshList[GEO_WALL] = MeshBuilder::GenerateOBJ("Wall", "OBJ//Wall.obj");
	meshList[GEO_WALL]->textureID = LoadTGA("Image//Wall.tga");

	//Floor
	meshList[GEO_FLOOR] = MeshBuilder::GenerateOBJ("model1", "OBJ//Floor.obj");
	meshList[GEO_FLOOR]->textureID = LoadTGA("Image//Floor.tga");
	meshList[GEO_FLOOR]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_FLOOR]->material.kDiffuse.Set(0.9f, 0.9f, 0.9f);
	meshList[GEO_FLOOR]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_FLOOR]->material.kShininess = 5.f;

	//Chest
	meshList[GEO_CHEST] = MeshBuilder::GenerateOBJ("Treasure Chest", "OBJ//Treasurechest.obj");
	meshList[GEO_CHEST]->textureID = LoadTGA("Image//Treasurechest.tga");

	//Blade
	meshList[GEO_BLADE] = MeshBuilder::GenerateOBJ("Blade", "OBJ//Blade.obj");
	//Text
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");

	FramesPerSec = 0;
}

static float ROT_LIMIT = 45.f;
static float SCALE_LIMIT = 5.f;
static float LSPEED = 10.F;


void Assignment3::Update(double dt)
{
	FramesPerSec = 1 / dt;
	if (Application::IsKeyPressed('1')) //enable back face culling
		glEnable(GL_CULL_FACE);
	if (Application::IsKeyPressed('2')) //disable back face culling
		glDisable(GL_CULL_FACE);
	if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode

	camera.Update(dt);

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


}
void Assignment3::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0) //Proper error Check
		return;
	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i*1.0f, 0, 0); //1.0f is spacing of each char
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		mesh->Render((unsigned)text[i] * 6, 6);

	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}
void Assignment3::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0) //Proper error Check
		return;
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 0);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i*1.0f, 0, 0); //1.0f is spacing of each char
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		mesh->Render((unsigned)text[i] * 6, 6);

	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}
void Assignment3::RenderSkyBox()
{
	modelStack.PushMatrix();//For da entire skybox

	modelStack.Scale(1000, 1000, 1000);
	//top
	modelStack.PushMatrix();
	modelStack.Translate(0, 0.5, 0);
	modelStack.Rotate(180, 0.f, 0.f, 1.f);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();

	//bottom
	modelStack.PushMatrix();
	modelStack.Translate(0, -0.5, 0);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();

	//Left
	modelStack.PushMatrix();
	modelStack.Translate(-0.5, 0.0, 0);
	modelStack.Rotate(180, 0.f, 1.f, 0.f);
	modelStack.Rotate(90, 0.f, 0.f, 1.f);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	//Right
	modelStack.PushMatrix();
	modelStack.Translate(0.5, 0.0, 0);
	modelStack.Rotate(90, 0.f, 0.f, 1.f);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	//front
	modelStack.PushMatrix();
	modelStack.Translate(0.0f, 0.0, -0.5f);
	modelStack.Rotate(90, 0.f, 1.f, 0.f);
	modelStack.Rotate(90, 0.f, 0.f, 1.f);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	//Back
	modelStack.PushMatrix();
	modelStack.Translate(0.0f, 0.0, 0.5f);
	modelStack.Rotate(-90, 0.f, 1.f, 0.f);
	modelStack.Rotate(90, 0.f, 0.f, 1.f);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	modelStack.PopMatrix();
}
void Assignment3::Render()
{
	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//These will be replaced by matrix stack soon
	Mtx44 model;
	Mtx44 view;
	Mtx44 projection;

	model.SetToIdentity();
	//Set view matrix using camera settings
	view.SetToLookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
		);

	//Set projection matrix to perspective mode
	projection.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f); //FOV, Aspect Ratio, Near plane, Far plane


	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z);
	modelStack.LoadIdentity();

	Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
	glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);


	RenderMesh(meshList[GEO_AXES], false);

	modelStack.PushMatrix();
	modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	modelStack.Scale(0.5f, 0.5f, 0.5f);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	RenderTextOnScreen(meshList[GEO_TEXT], "Hello World", Color(1, 0, 0), 3, 3, 0);

	//Loads the Skybox
	RenderSkyBox();

//Floor
	for (int i = 20; i > -20; i -= 1)
	{
		for (int z = 20; z > -20; z -= 1)
		{
			modelStack.PushMatrix();
			modelStack.Scale(1.f, 1.f, 1.f);
			modelStack.Translate(i, -1.0f, z);
			RenderMesh(meshList[GEO_FLOOR], true);
			modelStack.PopMatrix();
		}

	}

	modelStack.PushMatrix();
	modelStack.Scale(.6f, .6f, .6f);
	modelStack.Translate(7.f, .0f, 3.f);
	modelStack.Rotate(180, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_CHEST], false);
	modelStack.PopMatrix();

	
	modelStack.PushMatrix();
	modelStack.Scale(1.f, 1.f, 0.5f);
	modelStack.Translate(5.f, .0f, 5.f);
	modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_WALL], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(1.f, 1.f, 0.5f);
	modelStack.Translate(5.f, .0f, -5.f);
	modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_WALL], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(10.f, 4.4f, 0.f);
	modelStack.Rotate(90.f, 0.f, .0f, 1.f);
	modelStack.Scale(0.9f, 2.95f, 0.57f);
	RenderMesh(meshList[GEO_WALL], false);
	modelStack.PopMatrix();

	//GIMME DAT ITEM
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(0.f, .0f, 0.f);
	modelStack.Scale(5.6f, 5.6f, 5.6f);
	RenderMesh(meshList[GEO_CHEST], false);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);

	//GIMME DAT ITEM
	glDisable(GL_DEPTH_TEST);
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(60.f, 10.0f, 0.f);
	modelStack.Scale(2.f, 2.f, 2.f);
	modelStack.Rotate(-90, 1.f, 0.f, 0.f);
	modelStack.Rotate(90, 0.f, 0.f, 1.f);
	RenderMesh(meshList[GEO_BLADE], false);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);

	std::stringstream FramesPerSecond;
	FramesPerSecond << "FPS" << FramesPerSec;

	RenderTextOnScreen(meshList[GEO_TEXT], FramesPerSecond.str(), Color(0, 0, 0), 3, 1, 19);
}

void Assignment3::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
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
	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render(); //this line should only be called once 
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}
void Assignment3::Exit()
{
	// Cleanup VBO here
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}