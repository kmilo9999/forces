#pragma once
#include "Particle.h"
#include "Shader.h"
class ParticleForceGenerator
{
public:
	
	virtual void UpdateForce(Particle* particle, float t) = 0;
	virtual void Render(Particle* particle, Shader shader) = 0;
};

