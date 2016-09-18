/* Start Header *****************************************************************/
/*!
\file Particle.cpp
\author Camilo Diaz
\e-mail c.diaz@digipen.edu
\date 12/10/2015
\Brief Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
*/
/* End Header *******************************************************************/
#include "Particle.h"


Particle::Particle() :mesh(nullptr), Mass(0.0f)
{
	integrator = new EulerSemiImplicit();

}

Particle::Particle(Mesh* nmesh, float mass) :mesh(nmesh), Mass(mass)
{
	integrator = new EulerSemiImplicit();
}

Particle::~Particle()
{
	delete integrator;
}

void Particle::SetMass(float mass)
{
	Mass = mass;
}

float Particle::GetMass()
{
	return Mass;
}


void  Particle::AddForce(Vector3& force)
{
	ForceAccumlated += force;
}

Vector3 Particle::GetPosition()
{
	return mesh->transformation.Position;
}

void Particle::Integrate(float dt)
{
	this->Acceleration = ForceAccumlated * (1/Mass);
	integrator->UpdatePosition(this,dt);
	ClearAccumulator();
}

void Particle::SetPosition(Vector3& position)
{
	mesh->transformation.Position = position;
}

void Particle::ClearAccumulator()
{
	ForceAccumlated.Zero();
}