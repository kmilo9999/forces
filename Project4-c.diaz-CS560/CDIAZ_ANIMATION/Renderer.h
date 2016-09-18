/* Start Header *****************************************************************/
/*!
\file Render.h
\author Camilo Diaz
\e-mail c.diaz@digipen.edu
\date 9/24/2015
\Brief Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
*/
/* End Header *******************************************************************/

#pragma once

#include "Shader.h"
#include "Camera.h"
#include <vector>
#include "Skinnedmesh.h"
#include "Curve.h"
#include <fstream>
#include "Mesh.h"
#include "Model.h"
#include "Particle.h"
#include "ParticleForceRegistry.h"
#include "ParticleSpring.h"
#include "ParticleAnchoredSpring.h"
#include "ParticleGravity.h"

#define NUM_CUBES_X 3
#define NUM_CUBES_Y 3
#define NUM_LAYERS_Z 3
#define SPRING_K 50.0f
#define SPRING_REST 5.0f

class Renderer
{
public:
	Renderer();
	Renderer( float screenWidth, float ScreenHeight);
	~Renderer();
	void Prepare();
	
	void RenderModel(Shader shader);
	void RenderBones(Shader shader);
	void RenderCurve(Shader shader);
	void RenderMeshes(Shader shader);
	void RenderCustomModel(Shader shader);
	void RenderDistanceLine(Shader shader);
	void RenderSprings(Shader shader);

	bool InitMesh();
	void InitCurves();
	void InitWorld();
	void InitModel();
	void InitDistanceLine();
	void InitParticleSystem();

	void UpdateEntities(Shader shader, float dt, float dt2);
	void UpdateBones(Shader shader, float dt);
	void UpdateModel( float dt);
	void UpdateCustomModel(float dt);
	void UpdateMeshes(float dt);
	void SetCamera(Camera* );
	Camera* GetCamera();

	float GetScreenWidth();
	void SetScreenWidth(float);

	float GetScreenHeight();
	void SetScreenHeight(float);
	void HandleKeyInput(bool* keys,float dt);
	
	unsigned int GetSelectedMesh();
	void SetSelectedMesh(unsigned int);

	void ReinitMotion();

	Skinnedmesh* GetMesh();

	Model* GetModel();
	
	Vector2 circle;
	bool targetMoving;

	void ModifyTargetMovement();
	void SetGravity();
private:
	float FieldOfVIew;
	float NearPlane;
	float FarPLane;
	float ScreenWidth;
	float ScreenHeight;
	Camera* camera;
	Mesh* target;
	//SkinnedMesh m_mesh;
	std::vector<Skinnedmesh*> m_mesh;
	std::vector<Curve*> curves;
	Curve* ConcatenatedCurve;
	std::vector<Mesh*> meshes;
	std::vector<Particle*> WorldParticles;
	ParticleForceRegistry registry;
	Model* model;
	Mesh* cube;
	Particle* anchor1;
	Particle* anchor2;
	Transformation WorldTranformation;
	ofstream Myfile;
	ofstream Myfile2;
	unsigned int selectedMesh;
	void ReadForces();
	void CreatForces();
	void CreateAnchors();
	
	void ConcatenateCurves();
	Quaternion GetRotationTo(Vector3& orig, Vector3& dest, Vector3& fallbackAxis);
};

