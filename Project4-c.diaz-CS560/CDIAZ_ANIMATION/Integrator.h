/* Start Header *****************************************************************/
/*!
\file Integrator.h
\author Camilo Diaz
\e-mail c.diaz@digipen.edu
\date 12/10/2015
\Brief Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
*/
/* End Header *******************************************************************/
#pragma once
#include "Math/Vector3.h"
#include "Particle.h"


class Particle;


class Integrator
{
public:

	virtual void UpdatePosition(Particle* particle, float t) = 0;

};

