/* Start Header *****************************************************************/
/*!
\file ParticleForceRegistry.cpp
\author Camilo Diaz
\e-mail c.diaz@digipen.edu
\date 12/10/2015
\Brief Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
*/
/* End Header *******************************************************************/

#include "ParticleForceRegistry.h"


ParticleForceRegistry::ParticleForceRegistry() :useGravity(false)
{
}


ParticleForceRegistry::~ParticleForceRegistry()
{
	
	for (size_t i = 0; i != registrations.size(); ++i)
	{
		delete registrations[i].force;
	}
}

void ParticleForceRegistry::UpdateForces(float duration)
{
	
	for (size_t i =0; i != registrations.size(); i++)
	{
		Particle* pr = registrations[i].particle;
		if (ParticleGravity* v = dynamic_cast<ParticleGravity*>(registrations[i].force)) 
		{

			if (useGravity)
			{
				registrations[i].force->UpdateForce(pr, duration);
			}
		}
		else
		{
			registrations[i].force->UpdateForce(pr, duration);
		}
	
	}
	
}

void ParticleForceRegistry::Render(Shader shader)
{
	for (size_t i = 0; i != registrations.size(); i++)
	{
		
		registrations[i].force->Render(registrations[i].particle,shader);
		//cout << "force : " << pr->ForceAccumlated << "for particle " << i << "	";
	}
}

void ParticleForceRegistry::Add(Particle* particle, ParticleForceGenerator *fg)
{
	ParticleForceRegistry::ParticleForceRegistration registration;
	registration.particle = particle;
	registration.force = fg;
	registrations.push_back(registration);
}

void ParticleForceRegistry::Remove(Particle* particle, ParticleForceGenerator *fg)
{
	Registry::iterator i = registrations.begin();
	for (; i != registrations.end(); i++)
	{
		if (i->force == fg)
		{
			registrations.erase(i);
		}
	}
}

void ParticleForceRegistry::Clear()
{
	registrations.clear();
}

void ParticleForceRegistry::ToogleGravity()
{
	cout << "Gravity activated" << endl;
	useGravity = !useGravity;
}