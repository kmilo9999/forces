/* Start Header *****************************************************************/
/*!
\file EulerSemiImplicit.cpp
\author Camilo Diaz
\e-mail c.diaz@digipen.edu
\date 12/10/2015
\Brief Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
*/
/* End Header *******************************************************************/

#include "EulerSemiImplicit.h"


EulerSemiImplicit::EulerSemiImplicit()
{
}


EulerSemiImplicit::~EulerSemiImplicit()
{
}

void EulerSemiImplicit::UpdatePosition(Particle* particle, float t)
{
	
	particle->Velocity += (particle->Acceleration * t);
	//apply damping
	particle->Velocity = particle->Velocity * 0.98f;
	Vector3 tempVel = particle->Velocity;
	particle->mesh->transformation.Position += (tempVel * t);
	

}



