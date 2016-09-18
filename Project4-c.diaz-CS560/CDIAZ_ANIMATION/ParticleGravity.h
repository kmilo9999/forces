/* Start Header *****************************************************************/
/*!
\file ParticleGravity.h
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

class ParticleGravity :public ParticleForceGenerator
{
public:
	ParticleGravity();
	ParticleGravity(Vector3 gravity);
	~ParticleGravity();
	void UpdateForce(Particle* particle, float t) ;
	void Render(Particle* particle, Shader shader);
private:
	Vector3 Gravity;

};

