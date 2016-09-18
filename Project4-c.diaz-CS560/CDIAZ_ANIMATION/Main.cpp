/* Start Header *****************************************************************/
/*!
\file Main.cpp
\author Camilo Diaz
\e-mail c.diaz@digipen.edu
\date 9/24/2015
\Brief Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
*/
/* End Header *******************************************************************/


#include "OpenglGL.h"
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include "Framework.h"


using namespace std;



bool firstMouse = true;


/*
	call back method to receive the keyboard events
*/
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		Framework::GetInstance()->running = false;
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	else{
		Framework::GetInstance()->KeyPressedEvent(key, action);
	}


}


/*
	Main funtion
*/
int main(void)
{
	

	GLFWwindow* window;
	if (!glfwInit())
		return EXIT_FAILURE;

	window = glfwCreateWindow((int)Framework::GetInstance()->GetWindowSize().x, (int)Framework::GetInstance()->GetWindowSize().y, "CDIAZ", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return EXIT_FAILURE;
	}

	
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	


	Framework::GetInstance()->SetWindow(window);

	Framework::GetInstance()->Init();
	Framework::GetInstance()->Run();

	


	return 0;
}