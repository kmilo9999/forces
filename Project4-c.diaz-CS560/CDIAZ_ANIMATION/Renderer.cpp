/* Start Header *****************************************************************/
/*!
\file Render.cpp
\author Camilo Diaz
\e-mail c.diaz@digipen.edu
\date 9/24/2015
\Brief Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
*/
/* End Header *******************************************************************/
#include "Renderer.h"
#include "Math/Math.h"

#define NUM_CONES 5
#include <iostream>
#include <fstream>
/*
	Default Constructor of the Render class 
*/
Renderer::Renderer() :ScreenWidth(640.0f), ScreenHeight(480.0f), FieldOfVIew(70.0f), NearPlane(0.1f), FarPLane(1000.0f),
						targetMoving(true)
{
	selectedMesh = 0;
	Myfile.open("ArcLengthTable.txt");
	Myfile2.open("Debug.txt");
}

/*
 Constructor with parameters of the Render class
*/
Renderer::Renderer( float screenWidth, float screenHeight) : ScreenWidth(screenWidth), ScreenHeight(screenHeight),
															FieldOfVIew(70.0f), NearPlane(0.1f), FarPLane(1000.0f),
															targetMoving(true)
{
	selectedMesh = 0;
	Myfile.open("ArcLengthTable.txt");
	Myfile2.open("Debug.txt");
}

/*
Destructor of the Render class
*/
Renderer::~Renderer()
{
	auto size = m_mesh.size();
	for (int i = 0; i < (int)size; ++i)
	{
		m_mesh[i]->Clear();
		delete m_mesh[i];
	}
	for (size_t i = 0; i < WorldParticles.size(); ++i)
	{
		delete WorldParticles[i];
	}
	
	for (size_t i = 0; i < meshes.size(); ++i)
	{
		delete meshes[i];
	}

	delete cube;
	Myfile2.close();
}

/*
	Clear the Buffers and teh color of the screen
*/
void Renderer::Prepare()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5f, 0.69f, 1.0f, 1.0f);
	
}




/*
	Render the model in the screen
*/
void Renderer::RenderModel( Shader shader)
{
	shader.Use();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	GLuint textureSamplerID = glGetUniformLocation(shader.GetProgramID(), "textureSampler");
	glUniform1i(textureSamplerID, 0);
	
	Matrix4 ViewMatrix = camera->GetMatrix();
	
	//Matrix4 ProjectionMatrix = Math::Perspective(45.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
	
	
	//m_mesh.transformation.Orientation = Math::AngleDegreesAxis(90.0f, Vector3(-1, 0, 0));
	
	Matrix4 ModelMatrix;
	
	ModelMatrix = Math::Translate(m_mesh[selectedMesh]->transformation.Position) *
		Math::QuaterinionToMatrix(m_mesh[selectedMesh]->transformation.Orientation) *
		Math::Scale(m_mesh[selectedMesh]->transformation.Scale);



	Matrix4 mvpMatrix = ViewMatrix * ModelMatrix;

	int loc = shader.GetUniformLocation("mpvMatrix");

	glUniformMatrix4fv(loc, 1, GL_FALSE, mvpMatrix[0].data);	
	shader.SetUniformLocation("phongSpecular", Vector3(1.0, 1.0, 1.0));
	shader.SetUniformLocation("phongShininess", 50);
	shader.SetUniformLocation("lightAmbient", Vector3(0.5f, 0.5f, 0.5f));
	shader.SetUniformLocation("lightValue", Vector3(0.8f, 0.8f, 0.8f));
	shader.SetUniformLocation("lightPos", Vector3(0, 100.0f, 100.0f));
	shader.SetUniformLocation("eyePos", camera->Transform.Position);
	shader.SetUniformLocation("Model", ModelMatrix);
	
	m_mesh[selectedMesh]->Render();

	shader.Unuse();
	
}

/*
	Render the bones of the skinnedmesh in the screen
*/
void Renderer::RenderBones(Shader shader)
{
	
	shader.Use();
	Matrix4 ModelMatrix;

	ModelMatrix = Math::Translate(m_mesh[selectedMesh]->transformation.Position) *
		Math::QuaterinionToMatrix(m_mesh[selectedMesh]->transformation.Orientation) *
		Math::Scale(m_mesh[selectedMesh]->transformation.Scale);
	shader.SetUniformLocation("model", ModelMatrix);
	// Pass them to the shaders
	shader.SetUniformLocation("camera", camera->GetMatrix());	

	m_mesh[selectedMesh]->RenderBones();
	shader.Unuse();

}

/*
Render the parameterized curves in the screen
*/
void Renderer::RenderCurve(Shader shader)
{
	shader.Use();
	
	glEnable(GL_DEPTH_TEST);
	for (size_t i = 0; i < curves.size(); ++i)
	{
		Matrix4 ModelMatrix;

		ModelMatrix = Math::Translate(curves[i]->transformation.Position) *
			Math::QuaterinionToMatrix(curves[i]->transformation.Orientation) *
			Math::Scale(curves[i]->transformation.Scale);
		shader.SetUniformLocation("model", ModelMatrix);
		// Pass them to the shaders
		shader.SetUniformLocation("camera", camera->GetMatrix());
		curves[i]->Render();

	}
	
	shader.Unuse();
}

/*
	Set the camera
*/
void Renderer::SetCamera(Camera* cam)
{
	camera = cam;
}

/*
Returns the camera
*/
Camera* Renderer::GetCamera()
{
	return camera;
}

/*
Returns the Width of the sceen 
*/
float Renderer::GetScreenWidth()
{
	return ScreenWidth;
}

/*
Set the Width of the sceen
*/
void Renderer::SetScreenWidth(float width)
{
	ScreenWidth = width;
}

/*
Returns the height of the screen
*/
float Renderer::GetScreenHeight()
{
	return ScreenHeight;
}

/*
Set the height of the screen
*/
void Renderer::SetScreenHeight(float height)
{
	ScreenHeight = height;
}

/*
	Initialize the skinnedmeshes and the plane
*/
bool Renderer::InitMesh()
{

	Skinnedmesh* model1 = new Skinnedmesh();
	if (!model1->LoadMesh("assets/yodan.x")) {

			printf("Mesh yodan load failed\n");
			return false;
	}
	
	/*Mesh* model2 = new Mesh();
	if (!model2->LoadMesh("ArmyPilot/ArmyPilot.x")) {

			printf("Mesh ArmyPilot load failed\n");
			return false;
	}*/

	m_mesh.push_back(model1);
	//m_mesh.push_back(model2);
		
		m_mesh[0]->transformation.Scale = Vector3(0.1f, 0.1f, 0.1f);
		//m_mesh[0]->transformation.Position = curves[0]->GetCurvePairPoints()[0].GetPoint();
		m_mesh[0]->animation.time = 10.0f;
		return true;
}


void Renderer::InitModel()
{
	Mesh* cone = new Mesh();
	cone->LoadFromFle("assets/cone3.obj");
	cone->transformation.Position = Vector3(0.0f, 0.0f, 0.0f);

	target = cone->Clone();

	Texture* textureTarget = new Texture(GL_TEXTURE_2D, "assets/tron.jpg");
	if (textureTarget->Load())
		cone->AddTexture(textureTarget);
	else
		cout << "Texture not loaded" << endl;
	
	//cone->transformation.Orientation = Math::AngleDegreesAxis(30, Vector3(1.0f, 1.0f, 1.0f));
	Node* rootNode = new Node(cone);
	rootNode->IsRoot = true;
	rootNode->parent = nullptr;

	Node* parentNode = rootNode;
	for (int i = 0; i < NUM_CONES; ++i)
	{
		Mesh* childMesh = cone->Clone();
		Transformation transform;
		transform.Position = Vector3(0.0f, 2.0f, 0.0f);
		childMesh->transformation = transform;
		Node* childNode = new Node(childMesh);
		childNode->parent = parentNode;
		parentNode->Childs.push_back(childNode);
		parentNode = childNode;
	}
	
	

	model = new Model(rootNode);
	//model->Initialize();
	

	
	Texture* texture = new Texture(GL_TEXTURE_2D, "assets/target.png");
	if (texture->Load())
		target->AddTexture(texture);
	
	target->transformation.Position = Vector3(7.0f, 5.0f, 7.0f);
	meshes.push_back(target);

	

	circle.x = target->transformation.Position.x;
	circle.y = target->transformation.Position.z;

	model->InitLineDistance();

	
	vector<Vector3> vertices = {
		Vector3(-0.5f, 0.0f, 0.5f),
		Vector3(-0.5f, 0.0f, -0.5f),
		Vector3(0.5f, 0.0f, -0.5f),
		Vector3(0.5f, 0.0f, 0.5f), };

	vector<GLuint> indices = {
		0, 1, 3,
		3, 2, 1
	};

	Mesh* plane = new Mesh(vertices, indices);

	plane->transformation.Scale = Vector3(50.0f, 50.0f, 50.0f);

	meshes.push_back(plane);
}


void Renderer::InitDistanceLine()
{
	model->InitLineDistance();
}

void Renderer::RenderDistanceLine(Shader shader)
{
	shader.Use();
	Matrix4 ModelMatrix;
	shader.SetUniformLocation("model", ModelMatrix);
	shader.SetUniformLocation("camera", camera->GetMatrix());
	model->RenderLineDistance(shader, target->transformation.Position);
	shader.Unuse();
}

void Renderer::RenderSprings(Shader shader)
{
	shader.Use();
	Matrix4 ModelMatrix;
	shader.SetUniformLocation("model", ModelMatrix);
	shader.SetUniformLocation("camera", camera->GetMatrix());
	registry.Render(shader);
	shader.Unuse();
	
}

void Renderer::RenderCustomModel(Shader shader)
{

	shader.Use();
	Matrix4 ViewMatrix = camera->GetMatrix();
	shader.SetUniformLocation("camera", camera->GetMatrix());
	model->Render(shader);
	shader.Unuse();
}


// Initialize the particle system
void Renderer::InitParticleSystem()
{
	float offset_x = -2.0f;
	float offset_y = 6.0 * NUM_CUBES_Y;
	float offset_z = 0.0f;

	cube = new Mesh();
	cube->LoadFromFle("assets/cube.obj");
	Texture* textureTarget = new Texture(GL_TEXTURE_2D, "assets/tron.jpg");
	if (textureTarget->Load())
		cube->AddTexture(textureTarget);

	for (int k = 0; k < NUM_LAYERS_Z; ++k)
	{
		for (int i = 0; i < NUM_CUBES_Y; ++i)
		{
			for (int j = 0; j < NUM_CUBES_X; ++j)
			{
				Mesh*cloneCube = cube->Clone();
				
				cloneCube->transformation.Position = Vector3(offset_x, offset_y, offset_z);
				offset_x += 6.0f;
				Particle* particle = new Particle(cloneCube, 1.0f);
				meshes.push_back(cloneCube);
				WorldParticles.push_back(particle);
				ParticleGravity* gravity = new ParticleGravity();
				registry.Add(particle, gravity);
			}
			offset_x = -2.0f;
			offset_y -= 6.0f;
		}
		offset_z -= 6.0f;
		offset_x = -2.0f;
		offset_y = 6.0 * NUM_CUBES_Y;
	}

	

	CreatForces();
	
	
	CreateAnchors();
	
	
}

// Create the spring forces that link the particles
void Renderer::CreatForces()
{

	/*ParticleAnchoredSpring* paSA =
	new ParticleAnchoredSpring(&WorldParticles[0]->mesh->transformation.Position, 180.5f, 5.0f);
	registry.Add(WorldParticles[1], paSA);*/

	for (int k = 0; k < NUM_LAYERS_Z; ++k)
	{
		for (int j = 0; j< NUM_CUBES_Y; ++j)
		{
			for (int i = 0; i < NUM_CUBES_X; ++i)
			{
				size_t pos = j * NUM_CUBES_X + i + k * (NUM_CUBES_X *NUM_CUBES_Y);
				size_t numlayer = k * (NUM_CUBES_X *NUM_CUBES_Y);
				size_t pos_layer = i * NUM_CUBES_X + j;

				int temp_pos = pos % NUM_CUBES_X;
				if (temp_pos == 0)
				{
					//Left extrem

					
					
					ParticleSpring* psA = new ParticleSpring(WorldParticles[pos + 1], SPRING_K, SPRING_REST);
					registry.Add(WorldParticles[pos], psA);


					//check last row
					int temp1 = (pos % (NUM_CUBES_X *NUM_CUBES_Y)) + NUM_CUBES_Y;
					if (temp1< NUM_CUBES_X *NUM_CUBES_Y)
					{
						ParticleSpring* psA = new ParticleSpring(WorldParticles[pos + NUM_CUBES_Y], SPRING_K, SPRING_REST);
						registry.Add(WorldParticles[pos], psA);

						ParticleSpring* psb = new ParticleSpring(WorldParticles[(pos + NUM_CUBES_Y) + 1], SPRING_K, SPRING_REST);
						registry.Add(WorldParticles[pos], psb);

					}
					int temp2 = (pos % (NUM_CUBES_X *NUM_CUBES_Y)) - NUM_CUBES_Y;
					if (temp2 >= 0)
					{
						ParticleSpring* psA = new ParticleSpring(WorldParticles[pos - NUM_CUBES_Y], SPRING_K, SPRING_REST);
						registry.Add(WorldParticles[pos], psA);

						ParticleSpring* psb = new ParticleSpring(WorldParticles[(pos - NUM_CUBES_Y) + 1], SPRING_K, SPRING_REST);
						registry.Add(WorldParticles[pos], psb);
					}



				}
				else if (pos % 3 == 1)
				{
					//center

					ParticleSpring* psA = new ParticleSpring(WorldParticles[pos + 1], SPRING_K, SPRING_REST);
					registry.Add(WorldParticles[pos], psA);

					ParticleSpring* psb = new ParticleSpring(WorldParticles[pos - 1], SPRING_K, SPRING_REST);
					registry.Add(WorldParticles[pos], psb);

					//check last row
					int temp1 = (pos % (NUM_CUBES_X *NUM_CUBES_Y)) + NUM_CUBES_Y;
					if (temp1< NUM_CUBES_X *NUM_CUBES_Y)
					{
						ParticleSpring* psA = new ParticleSpring(WorldParticles[pos + NUM_CUBES_Y], SPRING_K, SPRING_REST);
						registry.Add(WorldParticles[pos], psA);
						ParticleSpring* psb = new ParticleSpring(WorldParticles[(pos + NUM_CUBES_Y) + 1], SPRING_K, SPRING_REST);
						registry.Add(WorldParticles[pos], psb);
						ParticleSpring* psc = new ParticleSpring(WorldParticles[(pos + NUM_CUBES_Y) - 1], SPRING_K, SPRING_REST);
						registry.Add(WorldParticles[pos], psc);
					}
					int temp2 = (pos % (NUM_CUBES_X *NUM_CUBES_Y)) - NUM_CUBES_Y;
					if (temp2 >= 0)
					{
						ParticleSpring* psA = new ParticleSpring(WorldParticles[pos - NUM_CUBES_Y], SPRING_K, SPRING_REST);
						registry.Add(WorldParticles[pos], psA);
						ParticleSpring* psb = new ParticleSpring(WorldParticles[(pos - NUM_CUBES_Y) + 1], SPRING_K, SPRING_REST);
						registry.Add(WorldParticles[pos], psb);
						ParticleSpring* psc = new ParticleSpring(WorldParticles[(pos - NUM_CUBES_Y) - 1], SPRING_K, SPRING_REST);
						registry.Add(WorldParticles[pos], psc);
					}

				}
				else if (pos % 3 == 2)
				{
					//right extreme

					ParticleSpring* psA = new ParticleSpring(WorldParticles[pos - 1], SPRING_K, SPRING_REST);
					registry.Add(WorldParticles[pos], psA);


					//check last row
					int temp1 = (pos % (NUM_CUBES_X *NUM_CUBES_Y)) + NUM_CUBES_Y;
					if (temp1< NUM_CUBES_X *NUM_CUBES_Y)
					{
						ParticleSpring* psA = new ParticleSpring(WorldParticles[pos + NUM_CUBES_Y], SPRING_K, SPRING_REST);
						registry.Add(WorldParticles[pos], psA);
						ParticleSpring* psb = new ParticleSpring(WorldParticles[(pos + NUM_CUBES_Y) - 1], SPRING_K, SPRING_REST);
						registry.Add(WorldParticles[pos], psb);
					}
					int temp2 = (pos % (NUM_CUBES_X *NUM_CUBES_Y)) - NUM_CUBES_Y;
					if (temp2 >= 0)
					{
						ParticleSpring* psA = new ParticleSpring(WorldParticles[pos - NUM_CUBES_Y], SPRING_K, SPRING_REST);
						registry.Add(WorldParticles[pos], psA);
						ParticleSpring* psb = new ParticleSpring(WorldParticles[(pos - NUM_CUBES_Y) - 1], SPRING_K, SPRING_REST);
						registry.Add(WorldParticles[pos], psb);
					}
				}

			}
		}
	}
	ReadForces();
}

void Renderer::ReadForces()
{
	ifstream  myfile;
	myfile.open("forces.csv");
	string line;
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			size_t coma = line.find(",");
			string a = line.substr(0, coma);
			string b = line.substr(coma+1);
			int intA = std::stoi(a);
			int intB = std::stoi(b);
			ParticleSpring* psA = new ParticleSpring(WorldParticles[intA], SPRING_K, SPRING_REST);
			registry.Add(WorldParticles[intB], psA);
		}
		myfile.close();
	}

	myfile.close();


}

void Renderer::CreateAnchors()
{

	float offset_x = -2.0f;
	float offset_y = 6.0 * NUM_CUBES_Y;
	float offset_z = 0.0f;

	Mesh* meshAchor1 = cube->Clone();
	Mesh* meshAchor2 = cube->Clone();

	Texture* texture = new Texture(GL_TEXTURE_2D, "assets/target.png");
	texture->Load();
	
	meshAchor1->AddTexture(texture);
	meshAchor1->transformation.Position = Vector3(-6.0f, offset_y, offset_z);
	
	meshAchor2->AddTexture(texture);
	meshAchor2->transformation.Position = Vector3(14.0f, offset_y, offset_z);

	meshes.push_back(meshAchor1);
	meshes.push_back(meshAchor2);

	anchor1 = new Particle(meshAchor1, 1.0f);
	anchor2 = new Particle(meshAchor2, 1.0f);
	
	WorldParticles.push_back(anchor1);
	WorldParticles.push_back(anchor2);
	

	ParticleAnchoredSpring* paSA =
		new ParticleAnchoredSpring(&anchor1->mesh->transformation.Position, 180.5f, 5.0f);
	registry.Add(WorldParticles[0], paSA);

	ParticleAnchoredSpring* paSb =
		new ParticleAnchoredSpring(&anchor2->mesh->transformation.Position, 180.5f, 5.0f);
	registry.Add(WorldParticles[2], paSb);

}

/*
Initialize the curves
*/
void Renderer::InitCurves()
{
	
	std::vector<Vector3> ControlPoints1 =
	{

		Vector3(-25.0f, 0.0f, 1.5f), Vector3(-20.0f, 0.0f, 5.0f),
		Vector3(-15.0f, 0.0f, 10.0f), Vector3(-5.0f, 0.0f, 15.5f),
		Vector3(5.0f, 0.0f, 10.0f), Vector3(10.0f, 0.0f, 5.0f),
		Vector3(15.0f, 0.0f, 1.5f), Vector3(25.0f, 0.0f, -2.5f),
	};
	
	std::vector<Vector3> ControlPoints2 =
	{

		
		Vector3(25.0f, 0.0f, -2.5f), Vector3(10.0f, 0.0f, -10.0f),
		Vector3(5.0f, 0.0f, -15.5f), Vector3(-5.0f, 0.0f, -6.9f),
		Vector3(-10.0f, 0.0f, -2.5f), Vector3(-17.0f, 0.0f, 4.0f),
		Vector3(-25.0f, 0.0f, 5.5f)
	};

	curves.push_back(new Curve(ControlPoints1));
	curves.push_back(new Curve(ControlPoints2));
	

	for (size_t i = 0; i < curves.size(); ++i)
	{
		curves[i]->InitCurve(i);
	}
	
	//ConcatenateCurves();
}


/*
	Concatenate two or more ArcLength Tables according to the instructions of
	CS560
*/
void Renderer::ConcatenateCurves()
{
	// Concatenated curves
	
	float currentParametricValue = 0.0f;
	int indexCounter = 0;
	ArcLengthPair lastEntryInPreviosTable;
	vector<ArcLengthPair> arcLengthTable;
	
	for (size_t i = 0; i < curves.size(); ++i)
	{
		Curve* tempCurve = curves[i];
		for (size_t j = 0; j < tempCurve->GetArcLengthTable().size(); ++j)
		{
			ArcLengthPair currentArcLength = tempCurve->GetArcLengthTable()[j];
			

			ArcLengthPair arcLengthPair (
				lastEntryInPreviosTable.GetParametricValue() + currentArcLength.GetParametricValue(),
				lastEntryInPreviosTable.GetArcLength() + currentArcLength.GetArcLength());
			
			arcLengthTable.push_back(arcLengthPair);

			
		}
		lastEntryInPreviosTable = 
			arcLengthTable[arcLengthTable.size() - 1];
	}

	ConcatenatedCurve = new Curve();
	ConcatenatedCurve->SetArcLengthTable(arcLengthTable);
	
	//Normalize Table
	
	float normalizeParametricValue =
		ConcatenatedCurve->GetArcLengthTable()[ConcatenatedCurve->GetArcLengthTable().size() - 1].GetParametricValue();
	float normalizeArcLengthValue = 
		ConcatenatedCurve->GetArcLengthTable()[ConcatenatedCurve->GetArcLengthTable().size() - 1].GetArcLength();

	if (!Myfile.is_open())
	{
		Myfile.open("ArcLengthTable.txt");
	}

	for (size_t i = 0; i < ConcatenatedCurve->GetArcLengthTable().size(); ++i)
	{
		ArcLengthPair arcLength = ConcatenatedCurve->GetArcLengthTable()[i];
		
		float normalizedParametricU = arcLength.GetParametricValue() / normalizeParametricValue;
		float normalizedArcLength = arcLength.GetArcLength() / normalizeArcLengthValue;
		
		ConcatenatedCurve->SetParametricValueInIndex(i, normalizedParametricU);
		ConcatenatedCurve->SetArcLengtValueInIndex(i, normalizedArcLength);
		Myfile << ConcatenatedCurve->GetArcLengthTable()[i].GetParametricValue() << " " << ConcatenatedCurve->GetArcLengthTable()[i].GetArcLength() << endl;
		

	}
	Myfile.close();
	cout << "Concatenation completed" << endl;
}

void Renderer::HandleKeyInput(bool* keys,float dt)
{
	
	float meshVel = 10.0f;
	Vector3 velDir;

	if (keys[GLFW_KEY_UP])
	{
		anchor1->mesh->transformation.Position =
			anchor1->mesh->transformation.Position + Vector3(0.0f, 0.1f, 0.0f);
	}
	if (keys[GLFW_KEY_DOWN])
	{
		anchor1->mesh->transformation.Position =
			anchor1->mesh->transformation.Position + Vector3(0.0f, -0.1f, 0.0f);
	}
	
	if (keys[GLFW_KEY_RIGHT])
	{
		anchor1->mesh->transformation.Position =
			anchor1->mesh->transformation.Position + Vector3(0.1f, 0.0f, 0.0f);
	}
	
	if (keys[GLFW_KEY_LEFT])
	{
		anchor1->mesh->transformation.Position =
			anchor1->mesh->transformation.Position + Vector3(-0.1f, 0.0f, 0.0f);
	}


	if (keys[GLFW_KEY_I])
	{
		anchor2->mesh->transformation.Position =
			anchor2->mesh->transformation.Position + Vector3(0.0f, 0.1f, 0.0f);
	}
	if (keys[GLFW_KEY_K])
	{
		anchor2->mesh->transformation.Position =
			anchor2->mesh->transformation.Position + Vector3(0.0f, -0.1f, 0.0f);
	}

	if (keys[GLFW_KEY_L])
	{
		anchor2->mesh->transformation.Position =
			anchor2->mesh->transformation.Position + Vector3(0.1f, 0.0f, 0.0f);
	}

	if (keys[GLFW_KEY_J])
	{
		anchor2->mesh->transformation.Position =
			anchor2->mesh->transformation.Position + Vector3(-0.1f, 0.0f, 0.0f);
	}


}

/*
	Returns the index of the selectec mesh displayed in the screen
*/
unsigned int Renderer::GetSelectedMesh()
{
	return selectedMesh;
}

/*
Sets the index of the selectec mesh displayed in the screen
*/
void Renderer::SetSelectedMesh(unsigned int selected)
{
	selectedMesh = selected;
}

/*
 Returns  the selected mesh displayed in the screen
*/
Skinnedmesh* Renderer::GetMesh()
{
	return m_mesh[selectedMesh];
}

/*
	Updte the Transformations of the bones
*/
void Renderer::UpdateBones(Shader shader, float dt)
{
	
	std::vector<Matrix4>  Transforms;
	m_mesh[selectedMesh]->BoneTransform(dt, Transforms);

	auto size = Transforms.size();
	for (int i = 0; i < (int)size; i++) {
		shader.SetBoneTransform(i, Transforms[i]);
	}
	
}

/*
	Update the model transformations along the curve such as pace and control speed
	It uses the ArcLength Tables previously calculated
*/

/*
Update the positions along the curve of a model and the animation of the bones
*/
void Renderer::UpdateEntities(Shader shader, float dt, float RunningTime)
{
	shader.Use();
	
	/*m_mesh[selectedMesh]->Animate(dt);
	float currentTime = m_mesh[selectedMesh]->GetAnimation().accumulator / m_mesh[selectedMesh]->GetAnimation().time;
	UpdateModel( currentTime);


	float pace = 1.7f;
	float speed = EaseSpeed(currentTime, 2.5f / m_mesh[selectedMesh]->GetAnimation().time, 9.5f / m_mesh[selectedMesh]->GetAnimation().time);

	float animationSpeed = speed / pace;
	float animationTime = RunningTime * animationSpeed;*/

	//UpdateBones(shader, animationTime);

	UpdateBones(shader, RunningTime);
	
	shader.Unuse();
}


void Renderer::UpdateModel( float currentTime)
{
	
	ArcLengthPair arcLength = ConcatenatedCurve->GetArcLengthTable()[ConcatenatedCurve->GetArcLengthTable().size()-1];
	float totalDistance = arcLength.GetArcLength();

	float firstQuarter = totalDistance * 0.25f ;
	float lastQuarter = totalDistance * 0.75f ;

	float s = EaseParabolic(currentTime, 2.5f / m_mesh[selectedMesh]->GetAnimation().time, 9.5f / m_mesh[selectedMesh]->GetAnimation().time);
	//float s = Ease(currentTime, firstQuarter, lastQuarter);
	Myfile2 << "ArcLenght: " << s << endl;
	
	float u = ConcatenatedCurve->GetParameterValueByArchLength(s);
	
	if (u > 1.0f)
	{
		u = 1.0f;
	}
	
	Myfile2 << "U : " << u << endl;
	unsigned int index = (int)( u * curves.size());
	if(index >= curves.size())
	{
		index--;
	}

	Curve* tempCurve = curves[index ];

	float UDenormalizied = u * curves.size() - (index);
	Myfile2 << "UDenormalizied : " << UDenormalizied << endl;
	index = tempCurve->GetLowEntryIndexOfParameterValue(UDenormalizied);
	
	CurvePointPair pointCurve = tempCurve->GetCurvePairPoints()[index];

	Myfile2 << "Point : " << pointCurve.GetPoint().x << " " << pointCurve.GetPoint().y << " " << pointCurve.GetPoint().z << endl;
	Myfile2 << " ----------- " << endl;

	m_mesh[selectedMesh]->transformation.Position = pointCurve.GetPoint();

	//Update Orientation
	unsigned  int futurePoint = index + 10;
	if (futurePoint >= tempCurve->GetCurvePairPoints().size())
	{
		futurePoint = tempCurve->GetCurvePairPoints().size() - 1;
	}
	
	CurvePointPair futurepointCurve = tempCurve->GetCurvePairPoints()[futurePoint];
	Vector3 W = futurepointCurve.GetPoint() - pointCurve.GetPoint();
	
	Quaternion q = GetRotationTo(Vector3(0, 0, 1), W, Vector3(0, 1, 0));
	m_mesh[selectedMesh]->transformation.Orientation = q;

	
}

/*
Reset the motion of the mesh aling the curve
*/
void Renderer::ReinitMotion()
{
	m_mesh[selectedMesh]->ResetAnimation();
}

/*
Render the plane in the screen
*/
void Renderer::RenderMeshes(Shader shader)
{
	shader.Use();
	for (size_t i = 0; i < meshes.size(); ++i)
	{
		glEnable(GL_DEPTH_TEST);
		Matrix4 ModelMatrix;
		ModelMatrix = Math::Translate(meshes[i]->transformation.Position) *
			Math::QuaterinionToMatrix(meshes[i]->transformation.Orientation) *
			Math::Scale(meshes[i]->transformation.Scale);
		shader.SetUniformLocation("model", ModelMatrix);
		shader.SetUniformLocation("hasTexture",meshes[i]->HasTexture());
		shader.SetUniformLocation("endEffector", false);
		shader.SetUniformLocation("isRoot", false);
		Matrix4 ViewMatrix = camera->GetMatrix();
		shader.SetUniformLocation("camera", camera->GetMatrix());

		for (size_t j = 0; j < meshes[i]->GetTextures().size(); ++j)
		{
			
			Texture* texture = meshes[i]->GetTextures()[j];
			texture->Bind(GL_TEXTURE0);
		}

		meshes[i]->Render();
		
	}
	shader.Unuse();
	
}

void Renderer::UpdateCustomModel(float dt)
{
	model->Update(dt);
	model->DoIK(target->transformation.Position);

}

Model* Renderer::GetModel()
{
	return model;
}

void Renderer::UpdateMeshes(float dt)
{

	registry.UpdateForces(0.01f);
	for (size_t i = 0; i < WorldParticles.size(); ++i)
	{
		
		WorldParticles[i]->Integrate(0.01f);
	
	}
}

/*
	Returns the orientation of the model according to CIO (center of interest)
*/
Quaternion Renderer::GetRotationTo(Vector3& orig, Vector3& dest, Vector3& fallbackAxis)
{
	// Based on Stan Melax's article in Game Programming Gems
	Quaternion q;
	// Copy, since cannot modify local
	Vector3 v0 = orig;
	Vector3 v1 = dest;
	v0 = Math::Normalize(v0);
	v1 = Math::Normalize(v1);

	float d = Math::Dot(v0, v1);
	// If dot == 1, vectors are the same
	if (d >= 1.0f)
	{
		return q;
	}
	if (d < (1e-6f - 1.0f))
	{
		if (fallbackAxis != Vector3(0, 0, 0))
		{
			// rotate 180 degrees about the fallback axis
			q = Math::AngleDegreesAxis(180.0f, fallbackAxis);
		}
		else
		{
			// Generate an axis
			Vector3 axis = Math::Cross(Vector3(1, 0, 0), v0);
			if (Math::Length(axis) == 0)
			{
				axis = Math::Cross(Vector3(0, 1, 0), v0);
			}
			axis = Math::Normalize(axis);
			q = Math::AngleDegreesAxis(180.0f, axis);
		}
	}
	else
	{
		float s = sqrt((1 + d) * 2);
		float invs = 1 / s;

		Vector3 c = Math::Cross(v0, v1);

		q.v.x = c.x * invs;
		q.v.y = c.y * invs;
		q.v.z = c.z * invs;
		q.w = s * 0.5f;

		q = Math::Nomalize(q);
	}
	return q;
}

void Renderer::ModifyTargetMovement()
{
	if (targetMoving)
	{
		circle.x = meshes[0]->transformation.Position.x;
		circle.y = meshes[0]->transformation.Position.z;
		targetMoving = false;
	}
	else{
		targetMoving = true;
	}
}

void Renderer::SetGravity()
{
	registry.ToogleGravity();
}