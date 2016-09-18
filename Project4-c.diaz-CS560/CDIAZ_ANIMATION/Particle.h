/* Start Header *****************************************************************/
/*!
\file Particle.h
\author Camilo Diaz
\e-mail c.diaz@digipen.edu
\date 12/10/2015
\Brief Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
*/
/* End Header *******************************************************************/

#pragma once
#include "Mesh.h"
#include "Math\Vector3.h"
#include "Integrator.h"
#include "EulerSemiImplicit.h"

class EulerSemiImplicit;

class Particle
{
public:
	Particle();
	Particle(Mesh* mesh, float mass );
	~Particle();

	void SetMass(float mass);
	float GetMass();

	
	void AddForce(Vector3& force);

	Vector3 GetPosition();
	void SetPosition(Vector3& position);
	void Integrate(float dt);
	void ClearAccumulator();

	Vector3 Velocity;
	Vector3 Acceleration;
	Mesh* mesh;
	Vector3 ForceAccumlated;
private: 

	
	float Mass;
	
	

	
	Integrator* integrator;
};

