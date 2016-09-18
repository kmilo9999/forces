/* Start Header *****************************************************************/
/*!
\file Node.cpp
\author Camilo Diaz
\e-mail c.diaz@digipen.edu
\date 9/24/2015
\Brief Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
*/
/* End Header *******************************************************************/
#include "Node.h"

/*
Constrcutor if the class Node
*/
Node::Node() :mesh(new Mesh()),IsRoot(false)
{
}

/*
Destructor of the class Node
*/
Node::~Node()
{
	for (size_t i = 0; i < Childs.size(); ++i)
	{
		delete Childs[i];
	}
}

/*
Constructor with parameters of the class Node
*/
Node::Node(Mesh* nmesh) :mesh(nmesh), IsRoot(false)
{

}

/*
Render the node in the screen
*/
void Node::Render(Shader shader)
{

	Transformation localTransformation =  mesh->transformation;

	Matrix4 ModelMatrix;
	ModelMatrix = Math::Translate(localTransformation.Position) *
		Math::QuaterinionToMatrix(localTransformation.Orientation) *
		Math::Scale(localTransformation.Scale);

	shader.SetUniformLocation("model", mesh->FinalTransformation);
	shader.SetUniformLocation("hasTexture", mesh->HasTexture());
	shader.SetUniformLocation("isRoot", IsRoot);

	if (IsLeaf())
	{
		shader.SetUniformLocation("endEffector",true);
	}
	else{
		shader.SetUniformLocation("endEffector", false);
	}

	for (size_t j = 0; j < mesh->GetTextures().size(); ++j)
	{

		Texture* texture = mesh->GetTextures()[j];
		texture->Bind(GL_TEXTURE0);
	}

	mesh->Render();


	for (size_t i = 0; i < Childs.size(); ++i)
	{
		Childs[i]->Render(shader);
	}
}

/*
Update the node in the screen
*/
void Node::Update(float dt)
{
	if (parent == nullptr)
	{
		Matrix4 translation = Math::Translate(mesh->transformation.Position);
		Matrix4 scale = Math::Scale(mesh->transformation.Scale);
		Matrix4 rotation = Math::QuaterinionToMatrix(mesh->transformation.Orientation);
		mesh->FinalTransformation = translation * rotation * scale;


	}
	else{
		Matrix4 translation = Math::Translate(mesh->transformation.Position);
		Matrix4 scale = Math::Scale(mesh->transformation.Scale);
		Matrix4 rotation = Math::QuaterinionToMatrix(mesh->transformation.Orientation);
		mesh->FinalTransformation = translation * rotation * scale;

		mesh->FinalTransformation = parent->mesh->FinalTransformation * mesh->FinalTransformation;
	}

	for (size_t i = 0; i < Childs.size(); ++i)
	{
		Childs[i]->Update(dt);
	}
}

/*
returns true if it has no childs (leaf)
*/
bool Node::IsLeaf()
{
	return Childs.empty();
}

Node* Node::GetEndEffector()
{
	
	if (IsLeaf())
	{
		return this;
	}
	else{
		for (size_t i = 0; i < Childs.size(); ++i)
		{
			Node* child = Childs[i];
			return child->GetEndEffector();
		}
	}
}

void Node::Rotate(float angle, Vector3& axis)
{
	//Transformation localTransformation = parentTransformation * mesh->transformation;
	mesh->transformation.Orientation = Math::AngleRadiansAxis(angle, axis);

	for (size_t i = 0; i < Childs.size(); ++i)
	{
		Node* child = Childs[i];
		child->Rotate(angle, axis);
	}
}

void Node::Rotate(Quaternion& q)
{
	Quaternion q2 = mesh->transformation.Orientation;
	mesh->transformation.Orientation = Math::Slerp(q2, q, 0.1f);

	for (size_t i = 0; i < Childs.size(); ++i)
	{
		Node* child = Childs[i];
		child->Rotate(q);
	}
}
