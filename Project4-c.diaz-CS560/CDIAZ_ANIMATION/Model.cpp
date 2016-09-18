/* Start Header *****************************************************************/
/*!
\file Model.cpp
\author Camilo Diaz
\e-mail c.diaz@digipen.edu
\date 9/24/2015
\Brief Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
*/
/* End Header *******************************************************************/

#include "Model.h"

/*
Constructor of the class model
*/
Model::Model() : change(0), distanceEndEffectorTarget(0)
{
}

/*
Constructor of the class model with parameters
rootNode is a pointer to the root node of the mesh
*/
Model::Model(Node* rootNode) : RootNode(rootNode), change(0), distanceEndEffectorTarget(0)
{
  endEffector = GetEndEffectors();
}

/*
Destrcutor of the class model with parameters
*/
Model::~Model()
{
	if (RootNode)
		delete RootNode;
}

/*
Returns the root node of the model
*/
Node* Model::GetRootNode()
{
	return RootNode;
}

/*
Sets the root node of the model
*/
void Model::SetRootNode(Node* rootNode)
{
	RootNode = rootNode;
}

/*
Render the hierarchical structor of the model
*/
void Model::Render(Shader shader)
{
	RootNode->Render(shader);
}

/*
Updates the hierarchical structor of the model
*/
void Model::Update(float dt)
{

	RootNode->Update(dt); 

}


/*
Initialize the buffers in order to draw a line between the enf effector and the target
*/
void Model::InitLineDistance()
{
	glGenVertexArrays(1, &vbaLine);
	glGenBuffers(1, &vboLine);
}


/*
Render a line between the enf effector and the target
*/
void Model::RenderLineDistance(Shader shader, Vector3 finalPosition)
{

	Matrix4 endEffecotrFinalTransformation = endEffector->mesh->FinalTransformation;
	Vector3 endEffecotrPosition(endEffecotrFinalTransformation[3][0], endEffecotrFinalTransformation[3][1],
		endEffecotrFinalTransformation[3][2]);
	
	VectorDistanceLine.push_back(finalPosition);

	VectorDistanceLine.push_back(endEffecotrPosition);

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
	glDrawArrays(GL_POINTS, 0, VectorDistanceLine.size());
	glDrawArrays(GL_LINES, 0, VectorDistanceLine.size());
	glDisable(GL_PROGRAM_POINT_SIZE);

	VectorDistanceLine.clear();

	
}

/*
Returns the end efector of the model
*/
Node* Model::GetEndEffectors()
{
	return RootNode->GetEndEffector();
}

/*
Initialize Inverse Knimatics in the hierarchical structor of the model
*/
void Model::DoIK(Vector3& finalPosition)
{
	CCD(endEffector->parent, finalPosition, endEffector);	
}

/*
CCD implementation
*/
void Model::CCD(Node* node, Vector3& finalPosition, Node* endEffector)
{
	
	Matrix4 endEffecotrFinalTransformation = endEffector->mesh->FinalTransformation;
	Vector3 endEffecotrPosition(endEffecotrFinalTransformation[3][0], endEffecotrFinalTransformation[3][1],
		endEffecotrFinalTransformation[3][2]);

	Matrix4 currentNodeFinalTransformation = node->mesh->FinalTransformation;
	Vector3 currenJoinPosition(currentNodeFinalTransformation[3][0], currentNodeFinalTransformation[3][1],
		currentNodeFinalTransformation[3][2]);

	Vector3 fromJoinToEndEffector = endEffecotrPosition - currenJoinPosition;
	Vector3 normalizedfromJoinToEndEffector = Math::Normalize(fromJoinToEndEffector);


	Vector3 jointToFinalPosition = finalPosition - currenJoinPosition;
	Vector3 normalizedJointToFinalPosition = Math::Normalize(jointToFinalPosition);
	
	float angle = Math::Dot(normalizedfromJoinToEndEffector, normalizedJointToFinalPosition);
	angle = acosf(angle);
	
	
	Vector3 cross = Math::Cross(normalizedfromJoinToEndEffector, normalizedJointToFinalPosition);
	Quaternion q = Math::AngleRadiansAxis(angle, cross);
	

	//node->Rotate(angle, cross);

	node->Rotate(q);
	
	node->Update(0.0);

	Matrix4 endEffecotrFinalTransformation2 = endEffector->mesh->FinalTransformation;
	Vector3 endEffecotrPosition2(endEffecotrFinalTransformation2[3][0], endEffecotrFinalTransformation2[3][1],
		endEffecotrFinalTransformation2[3][2]);

	distanceEndEffectorTarget = Math::Distance(endEffecotrPosition2, finalPosition);

	float distanceChange = Math::Distance(endEffecotrPosition2, endEffecotrPosition);
	

	if (Math::Distance(endEffecotrPosition2, finalPosition) > 3.5f)
	{
		if (distanceChange < 1)
		{
			if (change == 0.0f)
			{
			  change = distanceChange;
			}else
			{
			return;
			}

		}
		
		if (node != RootNode)
			CCD(node->parent , finalPosition, endEffector);
		else
			CCD(endEffector->parent, finalPosition, endEffector);
		change = 0.0f;

	}
	else
	{
		return;
	}

	
}