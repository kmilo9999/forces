/* Start Header *****************************************************************/
/*!
\file ParticleForceRegistry.h
\author Camilo Diaz
\e-mail c.diaz@digipen.edu
\date 12/10/2015
\Brief Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
*/
/* End Header *******************************************************************/

#pragma once
#include "Particle.h"
#include "ParticleForceGenerator.h"
#include "Shader.h"
#include "ParticleGravity.h"

class ParticleForceRegistry
{
public:
	ParticleForceRegistry();
	~ParticleForceRegistry();

protected:
	struct ParticleForceRegistration
	{
		Particle* particle;
		ParticleForceGenerator* force;
	};
	typedef std::vector<ParticleForceRegistration> Registry;
	Registry registrations;

private:
	bool useGravity;
public:
	
	void Add(Particle* particle, ParticleForceGenerator *fg);
	
	void Remove(Particle* particle, ParticleForceGenerator *fg);

	void ToogleGravity();

	void Clear();

	void UpdateForces(float duration);
	void Render(Shader shader);
};

