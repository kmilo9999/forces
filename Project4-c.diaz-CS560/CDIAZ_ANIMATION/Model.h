/* Start Header *****************************************************************/
/*!
\file Model.h
\author Camilo Diaz
\e-mail c.diaz@digipen.edu
\date 9/24/2015
\Brief Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
*/
/* End Header *******************************************************************/

#pragma once
#include <vector>
#include "Node.h"
using namespace std;


class Model
{
public:
	Model();
	Model(Node* rootNode);
	~Model();
	
	
	Node* GetRootNode();
	void SetRootNode(Node* rootNode);
	void Render(Shader shader);
	void RenderNode(Node* child, Transformation parentTransformation);
	void Update(float dt);
	Node* GetEndEffectors();
	void DoIK(Vector3& finalPosition);
	void CCD(Node* node, Vector3& finalPosition, Node* EndEffector);
	float distanceEndEffectorTarget;

	void InitLineDistance();
	void RenderLineDistance(Shader shader, Vector3 finalPosition);

private:
	Node* RootNode;
	float change;
	GLuint vboLine;
	GLuint vbaLine;
	vector<Vector3> VectorDistanceLine;
	Node* endEffector;
};

