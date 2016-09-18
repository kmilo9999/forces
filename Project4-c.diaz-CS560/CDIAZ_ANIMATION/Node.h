/* Start Header *****************************************************************/
/*!
\file Node.h
\author Camilo Diaz
\e-mail c.diaz@digipen.edu
\date 9/24/2015
\Brief Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
*/
/* End Header *******************************************************************/

#pragma once
#include "Mesh.h"
#include <vector>
#include "Shader.h"
#include "Math\Transformation.h"
#include "Math\Math.h"

using namespace std;

class Node
{
public:
	Node();
	Node(Mesh* mesh);
	~Node();
	Mesh* mesh;
	vector<Node*> Childs;// Every mesh include their own transformation
	void Render(Shader shader);
	Node* GetEndEffector();
	void Update(float dt);
	
	void Rotate(float angle,Vector3& axis);
	void Rotate(Quaternion& q);
	bool IsLeaf();
	bool IsRoot;
	Node* parent;
private:
	
};
