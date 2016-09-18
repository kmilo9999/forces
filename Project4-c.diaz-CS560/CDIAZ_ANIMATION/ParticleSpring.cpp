/* Start Header *****************************************************************/
/*!
\file ParticleSpring.cpp
\author Camilo Diaz
\e-mail c.diaz@digipen.edu
\date 12/10/2015
\Brief Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
*/
/* End Header *******************************************************************/

#include "ParticleSpring.h"
#include "Math\Math.h"

ParticleSpring::ParticleSpring() : Other(nullptr), K(0.0f),
RestDistance(0.0f), Damping(1.0f)
{
}


ParticleSpring::~ParticleSpring()
{
}

ParticleSpring::ParticleSpring(Particle* other, float springK, float resposition) :Other(other),K(springK),
RestDistance(resposition), Damping(1.0f)
{

}

ParticleSpring::ParticleSpring(Particle* right, Particle* left, float springK, float resposition) :Left(left), Other(right), K(springK),
RestDistance(resposition), Damping(1.0f)
{
	glGenVertexArrays(1, &vbaLine);
	glGenBuffers(1, &vboLine);
}

void ParticleSpring::SetDamping(float damnpig)
{
	Damping = damnpig;
}

float ParticleSpring::GetDamping()
{
	return Damping;
}

void ParticleSpring::UpdateForce(Particle* particle, float t)
{
	float point1 = (particle->mesh->transformation.Position.x + 0.5f);
	Vector3 left = Vector3(point1,
		particle->mesh->transformation.Position.y, particle->mesh->transformation.Position.z);
	float point2 = (Other->mesh->transformation.Position.x - 0.5f);
	Vector3 right = Vector3(point2,
		Other->mesh->transformation.Position.y, Other->mesh->transformation.Position.z);

	Vector3 force = particle->mesh->transformation.Position - Other->mesh->transformation.Position;

	float Length = Math::Length(force);
	float displacement = Length - RestDistance;
	force = Math::Normalize(force);

	Vector3 restoreForce = force*(displacement*K);

	Other->AddForce(restoreForce);
	


}

void ParticleSpring::Render(Particle* particle, Shader shader)
{
	Vector3 right = Other->mesh->transformation.Position;
	Vector3 left = particle->mesh->transformation.Position;

	VectorDistanceLine.push_back(right);

	VectorDistanceLine.push_back(left);
	shader.SetUniformLocation("anchor", 0);
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

