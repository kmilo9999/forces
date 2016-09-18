/* Start Header *****************************************************************/
/*!
\file ParticleAnchoredSpring.cpp
\author Camilo Diaz
\e-mail c.diaz@digipen.edu
\date 12/10/2015
\Brief Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
*/
/* End Header *******************************************************************/

#include "ParticleAnchoredSpring.h"
#include "Math\Math.h"

ParticleAnchoredSpring::ParticleAnchoredSpring()
{
}

ParticleAnchoredSpring::ParticleAnchoredSpring(Vector3 *anchor, float springConstant, float restDistance):
Anchor(anchor), K(springConstant), RestDistance(restDistance)
{
}

ParticleAnchoredSpring::~ParticleAnchoredSpring()
{
}

void ParticleAnchoredSpring::UpdateForce(Particle *particle, float duration)
{
	// Calculate the vector of the spring.
	Vector3 force = particle->mesh->transformation.Position;
	force -= *Anchor;
	// Calculate the magnitude of the force.
	float magnitude = Math::Length(force);
	magnitude = fabs(magnitude - RestDistance);
	magnitude *= K;
	// Calculate the final force and apply it.
	force = Math::Normalize(force);
	force *= -magnitude;
	particle->AddForce(force);
}

void ParticleAnchoredSpring::Render(Particle* particle, Shader shader)
{
	Vector3 right = particle->mesh->transformation.Position;
	Vector3 left = *Anchor;
	
	VectorDistanceLine.push_back(right);

	VectorDistanceLine.push_back(left);
	shader.SetUniformLocation("anchor", 1);
	glBindVertexArray(vbaLine);
	glBindBuffer(GL_ARRAY_BUFFER, vboLine);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(VectorDistanceLine[0]) * VectorDistanceLine.size(),
		&VectorDistanceLine[0],
		GL_STATIC_DRAW
		);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);


	glDisable(GL_DEPTH_TEST);

	glEnable(GL_PROGRAM_POINT_SIZE);
	glBindVertexArray(vbaLine);
	glDrawArrays(GL_LINES, 0, VectorDistanceLine.size());
	glDisable(GL_PROGRAM_POINT_SIZE);

	VectorDistanceLine.clear();
}