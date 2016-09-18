/* Start Header *****************************************************************/
/*!
\file ParticleAnchoredSpring.h
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
#include "Math\Vector3.h"
class ParticleAnchoredSpring : public ParticleForceGenerator
{
public:
	ParticleAnchoredSpring();
	ParticleAnchoredSpring(Vector3 *anchor, float springConstant, float restDistance);
	~ParticleAnchoredSpring();
	void UpdateForce(Particle *particle, float duration);
	void Render(Particle* particle, Shader shader);
private:
	
	Vector3 *Anchor;
	
	float K;

	float RestDistance;

	GLuint vboLine;
	GLuint vbaLine;
	vector<Vector3> VectorDistanceLine;
};

