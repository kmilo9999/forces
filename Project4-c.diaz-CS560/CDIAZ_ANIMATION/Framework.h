/* Start Header *****************************************************************/
/*!
\file Framework.h
\author Camilo Diaz
\e-mail c.diaz@digipen.edu
\date 9/24/2015
\Brief Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
*/
/* End Header *******************************************************************/
#pragma once
#include "OpenglGL.h"
#include <GLFW/glfw3.h>
#include "Math/Vector2.h"
#include "Renderer.h"
#include "Common.hpp"
#include <AntTweakBar.h>


class Framework
{
public:
	static Framework* GetInstance();
	//Framework();
	~Framework();
	void Init();
	void Run();
	GLFWwindow* GetWindow();
	void SetWindow(GLFWwindow*);
	void SetWindowSize(Vector2 v);
	Vector2 GetWindowSize();
	
	void KeyPressedEvent(int key, int action);
	void MouseMoveEvent(double  posx, double posy);
	bool running;
	bool keys[1024];
	bool showBones = false;
	TwBar *myBar;

	Renderer* GetRendered();
	bool reinitAnimationCounter;

private:
	Framework();
	void LoadMeshShaders();
	void LoadBonesShaders();
	void LoadCurveShaders();
	void LoadCustomModelShaders();
	void InitDebugGui();	
	void InitOpengl();
	void InitCamera();
	static void TwEventMouseButtonGLFW3(GLFWwindow* window, int button, int action, int mods);
	static void TwEventMouseMoveGLFW3(GLFWwindow* window, double xpos, double ypos);

	static Framework* instance;
	
	GLFWwindow* window;
	
	Vector2 WindowSize;
	Renderer* render;
	Shader meshShader;
	Shader bonesShader;
	Shader curveShader;
	Shader customMeshShader;

	Vector3 m_position;
	
	
	float fps;
	float mouseX;
	float mouseY;
	float mouseZ;

};

