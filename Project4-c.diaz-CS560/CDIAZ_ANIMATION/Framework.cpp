/* Start Header *****************************************************************/
/*!
\file Framework.cpp
\author Camilo Diaz
\e-mail c.diaz@digipen.edu
\date 9/24/2015
\Brief Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
*/
/* End Header *******************************************************************/

#include "Framework.h"
#include <stdlib.h>
#include <stdio.h>
#include "Shader.h"
#include "Math/Transformation.h"
#include <windows.h>


#define  TIME_STEP 1.0f/60.0f


void TW_CALL ShowBones(void *clientData);
void TW_CALL SetGravity(void *clientData);
void TW_CALL ReInitMotion(void *clientData);
void TW_CALL TargetMovement(void *clientData);
void TW_CALL SetModel(const void *value, void *clientData);
void TW_CALL GetModel(void *value, void *clientData);
Framework* Framework::instance(0);

/*
	Singlenton Intance of framework class
*/
Framework* Framework::GetInstance()
{
	if (!instance)
	{
		instance = new Framework();
	}
	return instance;
}

/*
  Contructor of the class Framework
*/
Framework::Framework() :window(), WindowSize(854, 480), mouseX(0.0f), mouseY(0.0f), mouseZ(0.0f)
{
	for (int i = 0; i < 1024; i++)
	{
		keys[i] = false;
	}
	reinitAnimationCounter = false;
}

/*
	Contructor of the class Framework
*/
Framework::~Framework()
{
	if (render)
	{
		delete render;
	}
	if (instance)
	{
		delete instance;
	}
	
}

/*
	Initialize all the components of the scene:
	Curves
	Plane
	SkinnedMeshes
	Camera
	Shaders
	Gui

*/
void Framework::Init()
{
	InitOpengl();
	
	
	render = new Renderer();
	render->InitCurves();
	//render->InitMesh();
	//render->InitModel();

	render->InitParticleSystem();

	InitDebugGui();

	InitCamera();
	LoadMeshShaders();
	LoadBonesShaders();
	LoadCurveShaders();
	LoadCustomModelShaders();
	
}

/*
	Run the Game Loop where updates all the componentes of the scene
*/
void Framework::Run()
{
	
	

	bool fullscreen = false;
	
	running = true;

	double accumulator = 0;
	double timeaccumulator = 0;
	double frameCounter = 0.0f;
	double prevTime = glfwGetTime();
	float RunningTime, prevRunningTime = 0.0f;
	double m_startTime = (double)GetTickCount();
	/* Loop until the user closes the window */
	glfwSwapInterval(1);
	while (running )
	{
		cout << "Position: " << render->GetCamera()->Transform.Position << endl;
		cout << "Orientation: " << render->GetCamera()->Transform.Orientation << endl;
		if (reinitAnimationCounter)
		{
 			 accumulator = 0;
			 timeaccumulator = 0;
			 frameCounter = 0.0f;
			 prevTime = glfwGetTime();
			 RunningTime, prevRunningTime = 0.0f;
			 m_startTime = (double)GetTickCount();
			reinitAnimationCounter = false;
		}

		//Handle window resize
		{
			int width, height;
			glfwGetWindowSize(window, &width, &height);
			WindowSize.x = (float)width;
			WindowSize.y = (float)height;
			glViewport(0, 0, (GLsizei)WindowSize.x, (GLsizei)WindowSize.y);

		}
		{
			if (glfwWindowShouldClose(window))
			{
				running = false;
			}
		}
		

		double currentTime = glfwGetTime();
		double dt = currentTime - prevTime;
		prevTime = currentTime;
		accumulator += dt;
		timeaccumulator += dt;
		render->HandleKeyInput(keys,(float)dt);

		while (accumulator >= TIME_STEP)
		{
			accumulator -= TIME_STEP;
			frameCounter += 1;
			render->GetCamera()->UpdateKey(keys, TIME_STEP);
			render->GetCamera()->UpdateMouse( window,TIME_STEP);
	
		}

		RunningTime = (float)((double)GetTickCount() - (double)m_startTime) / 1000.0f;
		float elapsedTime = (RunningTime - prevRunningTime) + 0.00001f;
		
		
		prevRunningTime = RunningTime;
		

		

		// Update Model, Bones and animations
		//render->UpdateEntities(meshShader, elapsedTime, RunningTime);
		//render->UpdateCustomModel((float)glfwGetTime());


		render->UpdateMeshes(RunningTime);

		render->Prepare();
		render->SetScreenWidth(WindowSize.x);
		render->SetScreenHeight(WindowSize.y);
		//render->RenderModel(meshShader);
		render->RenderMeshes(customMeshShader);
		render->RenderSprings(bonesShader);
		//render->RenderCustomModel(customMeshShader);
		//render->RenderDistanceLine(bonesShader);
		//render->RenderCurve(curveShader);
		
		if (showBones)
		{
			render->RenderBones(bonesShader);
		}

		frameCounter += 1;
		
		if (timeaccumulator > 1.0f)
		{
			fps = (float)(frameCounter / timeaccumulator );
			timeaccumulator -= 1.0f;
			frameCounter = 0.0f;
		}
		
		
		TwDraw();
		
		glfwSwapBuffers(window);
		
		/* Poll for and process events */
		glfwPollEvents();

	}

	glfwDestroyWindow(window);
	TwTerminate();
	delete render;
}

/*
	Initialize Opengl
*/
void Framework::InitOpengl()
{
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

	}


	glEnable(GL_DEPTH_TEST);
}


/*
	Initialize the Virtual Camera
*/
void Framework::InitCamera()
{
	Camera* camera = new Camera();
	
    camera->Transform.Position = Vector3(3.33333f, 67.8721f, 37.3296f);
		  camera->Transform.Orientation = Quaternion(-0.456822f, -0.00697113f, 0.00562094f, 0.889513f);
	camera->LookAt(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
	camera->FieldOfView = 45;

	render->SetCamera(camera);
}

/*
	Load SkinnedMesh Shaders
*/
void Framework::LoadMeshShaders()
{
	meshShader.CreateProgram();
	meshShader.CreateShaderFromFile("Shaders/vertexShader.glsl", ShaderType::VERTEX_SHADER);
	meshShader.CreateShaderFromFile("Shaders/fragmentShader.glsl", ShaderType::FRAGMENT_SHADER);
	meshShader.LinkProgram();

	if (!meshShader.IsLinked())
		throw runtime_error("Shader not Linked Properly");

	meshShader.Use();

	meshShader.SetBonesLocation();
	meshShader.Unuse();
}

/*
	Load Bones Shaders
*/
void Framework::LoadBonesShaders()
{
	bonesShader.CreateProgram();
	bonesShader.CreateShaderFromFile("Shaders/BonesVertex.glsl", ShaderType::VERTEX_SHADER);
	bonesShader.CreateShaderFromFile("Shaders/BonesFragment.glsl", ShaderType::FRAGMENT_SHADER);
	bonesShader.LinkProgram();

	if (!bonesShader.IsLinked())
		throw runtime_error("Shader not Linked Properly");

}

/*
Load Curves Shaders
*/
void Framework::LoadCurveShaders()
{
	curveShader.CreateProgram();
	curveShader.CreateShaderFromFile("Shaders/CurveVertexShader.glsl", ShaderType::VERTEX_SHADER);
	curveShader.CreateShaderFromFile("Shaders/CurveFragmentShader.glsl", ShaderType::FRAGMENT_SHADER);
	curveShader.LinkProgram();

	if (!curveShader.IsLinked())
		throw runtime_error("Shader not Linked Properly");
}

/*
Load Plane Shaders
*/
void Framework::LoadCustomModelShaders()
{
	customMeshShader.CreateProgram();
	customMeshShader.CreateShaderFromFile("Shaders/MeshVertexShader.glsl", ShaderType::VERTEX_SHADER);
	customMeshShader.CreateShaderFromFile("Shaders/MeshFragmentShader.glsl", ShaderType::FRAGMENT_SHADER);
	customMeshShader.LinkProgram();

	if (!customMeshShader.IsLinked())
		throw runtime_error("Shader not Linked Properly");
}

/*
	returns the instance of a GLFW window
*/
GLFWwindow* Framework::GetWindow()
{
	return window;
}

/*
	return the size of the window
*/
void Framework::SetWindowSize(Vector2 v)
{
	WindowSize = v;
}

/*
return the size of the window
*/
Vector2 Framework::GetWindowSize()
{
	return WindowSize;
}

/*
 Set a new dimension of the window
*/
void Framework::SetWindow(GLFWwindow* w)
{
	window = w;
}

/*
	Update teh state of the Keys 
*/
void Framework::KeyPressedEvent(int key, int action)
{
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

/*
	Update the position of the mouse in the engine
*/
void Framework::MouseMoveEvent(double  posx, double posy)
{
	render->GetCamera()->posX = posx;
	render->GetCamera()->posY = posy;
}

/*
	Return a Render Instance
*/
Renderer* Framework::GetRendered()
{
	return render;
}

/*
	Enable or Disable the option to show the bones in screen
*/
void TW_CALL ShowBones(void *clientData)
{
	Framework *scene = static_cast<Framework *>(clientData); 
	scene->showBones = !scene->showBones;
}

/*
	Reset the motion of the model along a curve
*/
void TW_CALL ReInitMotion(void *clientData)
{
	Framework *scene = static_cast<Framework *>(clientData);
	scene->GetRendered()->ReinitMotion();
	scene->reinitAnimationCounter = true;
}

/*
Reset the motion of the model along a curve
*/
void TW_CALL SetModel(const void *value, void *clientData)
{
	Framework *scene = static_cast<Framework *>(clientData);
	int val = *(int*)value;
	scene->GetRendered()->SetSelectedMesh(val);	
	scene->GetRendered()->GetMesh()->selectedAnimation = 0;
}

/*
	Return the select model displayed in screen
*/
void TW_CALL GetModel(void *value, void *clientData)
{
	Framework *scene = static_cast<Framework *>(clientData);
	*(int*)value = scene->GetRendered()->GetSelectedMesh();
}


/*
Set the animation to run for the selected model
*/
void TW_CALL SetAnimation(const void *value, void *clientData)
{
	Framework *scene = static_cast<Framework *>(clientData);
	int val = *(int*)value;
	int numAni = scene->GetRendered()->GetMesh()->numAnimations;
	if (val <= numAni)
	{
		scene->GetRendered()->GetMesh()->selectedAnimation = val - 1;
	}
	
}

/*
Callback function for antweekbar
*/
void TW_CALL TargetMovement(void *clientData)
{
	Framework *scene = static_cast<Framework *>(clientData);
	scene->GetRendered()->ModifyTargetMovement();

}

/*
	Returns the animation to run for the selected model
*/
void TW_CALL GetAnimation(void *value, void *clientData)
{
	Framework *scene = static_cast<Framework *>(clientData);
	*(int*)value = scene->GetRendered()->GetMesh()->selectedAnimation;
}
void TW_CALL SetGravity(void *clientData)
{
	Framework *scene = static_cast<Framework *>(clientData);
	scene->GetRendered()->SetGravity();
}
/*
	Callback function for antweekbar
*/
void Framework::TwEventMouseButtonGLFW3(GLFWwindow* window, int button, int action, int mods)
{ 
	if (TwEventMouseButtonGLFW(button, action))
		return;
	
	double posiX, posiY;
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		
		glfwGetCursorPos(window, &posiX, &posiY);
		Framework::GetInstance()->GetRendered()->GetCamera()->previosX = posiX;
		Framework::GetInstance()->GetRendered()->GetCamera()->previosY = posiY;

		

		int width, height;
		glfwGetWindowSize(window, &width, &height);

		float x = float(2.0f * (float)posiX) / width - 1.0f;
		float y = 1.0f - (2.0f * (float)posiY) / height;
		float z = 1.0f;

		Vector3 ray_nds = Vector3(x, y, z);
		Vector4 ray_clip = Vector4(ray_nds.x, ray_nds.y, -1.0, 1.0);
		Matrix4 projectionMatrix = Framework::GetInstance()->GetRendered()->
			GetCamera()->GetProjection();
		Matrix4 viewMatrix = Framework::GetInstance()->GetRendered()->
			GetCamera()->GetProjection();
		
		Matrix4 inversePorjection = Math::Inverse(projectionMatrix);
		Matrix4 inverseView = Math::Inverse(viewMatrix);
		Vector4 ray_eye = inversePorjection * ray_clip;
		ray_eye = Vector4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);
		

		Vector4 ray_wor = inverseView * ray_eye;		

		Vector3 ray_word3(ray_wor.x, ray_wor.y, ray_wor.z);

		ray_word3 = Math::Normalize(ray_word3);

		Framework::GetInstance()->mouseX = ray_word3.x;
		Framework::GetInstance()->mouseY = ray_word3.y;
		Framework::GetInstance()->mouseZ = ray_word3.z;
	}

	
	
}

/*
Callback function for antweekbar
*/
void Framework::TwEventMouseMoveGLFW3(GLFWwindow* window, double xpos, double ypos)
{
	if (TwMouseMotion((int)xpos, (int)ypos))
		return;

	double  dx = xpos - Framework::GetInstance()->GetRendered()->GetCamera()->previosX;
	double  dy = ypos - Framework::GetInstance()->GetRendered()->GetCamera()->previosY;

	Framework::GetInstance()->GetRendered()->GetCamera()->eyeSpin += (float)dx / 2.0f;
	//Framework::GetInstance()->GetRendered()->GetCamera()->eyeTilt += dy / 2.0f;

	Framework::GetInstance()->GetRendered()->GetCamera()->previosX = xpos;
	Framework::GetInstance()->GetRendered()->GetCamera()->previosY = ypos;


}

/*
Initalize all the components in the GUI antweekbar
*/
void Framework::InitDebugGui()
{
	TwInit(TW_OPENGL_CORE, NULL);
	TwWindowSize((int)WindowSize.x, (int)WindowSize.y);
	glfwSetMouseButtonCallback(window, Framework::TwEventMouseButtonGLFW3);
	glfwSetCursorPosCallback(window, Framework::TwEventMouseMoveGLFW3);
	myBar = TwNewBar("Debug");
	TwDefine(" Debug size='300 350' ");
	
	
	TwAddVarRO(myBar, "FPS", TW_TYPE_FLOAT, &fps, " label='FPS' ");
	
	//TwAddVarRO(myBar, "Mouse X", TW_TYPE_FLOAT, &mouseX, " label='Mouse X' ");
	//TwAddVarRO(myBar, "Mouse Y", TW_TYPE_FLOAT, &mouseY, " label='Mouse Y' ");
	//TwAddVarRO(myBar, "Mouse Z", TW_TYPE_FLOAT, &mouseZ, " label='Mouse Z' ");

	//TwAddVarRO(myBar, "Pos X", TW_TYPE_FLOAT, &render->GetMesh()->transformation.Position.x, " label='Pos X' ");
	//TwAddVarRO(myBar, "Pos Y", TW_TYPE_FLOAT, &render->GetMesh()->transformation.Position.y, " label='Pos Y' ");
	//TwAddVarRO(myBar, "Pos Z", TW_TYPE_FLOAT, &render->GetMesh()->transformation.Position.z, " label='Pos Z' ");


	//TwAddVarRO(myBar, "Distance", TW_TYPE_FLOAT, &render->GetModel()->distanceEndEffectorTarget, " label='Distance Target' ");

	//TwAddButton(myBar, "Show Bones", (TwButtonCallback)ShowBones, this, " label='Show Bones' ");
	//TwAddButton(myBar, "Control Target", (TwButtonCallback)TargetMovement, this, " label='Control Target' ");
	//TwAddButton(myBar, "Re-init Motion", (TwButtonCallback)ReInitMotion, this, " label='Reset Motion' ");
	/*TwAddVarCB(myBar, "Model", TwDefineEnum("Model", NULL, 0),
		SetModel, GetModel, this,
		" enum='0 {Lizzard}, 1 {Police}' ");
	
	TwAddVarCB(myBar, "Animation", TwDefineEnum("Animation", NULL, 0),
		SetAnimation, GetAnimation, this,
		" enum=' 1 {1}, 2 {2} , 3 {3}' ");*/
	TwAddButton(myBar, "Use Gravity", (TwButtonCallback)SetGravity, this, " label='Toogle Gravity' ");
	TwAddButton(myBar, "Comment1", NULL, NULL, " label='USE AWSD KEYS TO MOVE THE CAMERA'");
	TwAddButton(myBar, "Comment2", NULL, NULL, " label='PRESS THE MOUSE MIDDLE BUTTON AND'"); 
	TwAddButton(myBar, "Comment3", NULL, NULL, " label='DRAG IT TO ROTATE THE VIEW\n'");
	TwAddButton(myBar, "Comment4", NULL, NULL, " label='PRESS THE CONTROL TARGER BUTTON'");
	TwAddButton(myBar, "Comment5", NULL, NULL, " label='TO MOVE THE TARGET AT WILL'");
	TwAddButton(myBar, "Comment6", NULL, NULL, " label='USE ARROW KEYS TO MOVE THE LEFT ANCHOR'");
	TwAddButton(myBar, "Comment7", NULL, NULL, " label='USE IJKL KEYS TO MOVE THE RIGHT ANCHOR'");
	
	
}
