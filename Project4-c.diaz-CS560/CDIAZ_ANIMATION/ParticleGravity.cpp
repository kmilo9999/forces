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

#include "ParticleGravity.h"


ParticleGravity::ParticleGravity() :Gravity(Vector3(0.0f,-9.8f,0.0f))
{
}

ParticleGravity::ParticleGravity(Vector3 gravity) : Gravity(gravity)
{

}

ParticleGravity::~ParticleGravity()
{
}

void ParticleGravity::UpdateForce(Particle* particle, float t)
{
	particle->AddForce(Gravity * particle->GetMass());
}

void ParticleGravity::Render(Particle* particle, Shader shader)
{

}