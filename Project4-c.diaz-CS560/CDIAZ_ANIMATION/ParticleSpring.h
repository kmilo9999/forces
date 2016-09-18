/* Start Header *****************************************************************/
/*!
\file ParticleSpring.h
\author Camilo Diaz
\e-mail c.diaz@digipen.edu
\date 12/10/2015
\Brief Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
*/
/* End Header *******************************************************************/

#pragma once
#include "ParticleForceGenerator.h"
#include "Particle.h"
#include "Math\Vector3.h"
#include "Shader.h"

class ParticleSpring :public ParticleForceGenerator
{
public:
	ParticleSpring();
	ParticleSpring(Particle* other, float springK, float RestDistance );
	ParticleSpring(Particle* right, Particle* left, float springK, float RestDistance);
	~ParticleSpring();
	
	void UpdateForce(Particle* particle, float t) ;
	void SetDamping(float damnpig);
	float GetDamping();
	
	void Render(Particle* particle, Shader shader);

private:

	Particle* Other;
	Particle* Left;
	float RestDistance;
	float K;
	GLuint vboLine;
	GLuint vbaLine;
	vector<Vector3> VectorDistanceLine;
	float Damping;
	

	
};

