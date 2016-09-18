/* Start Header *****************************************************************/
/*!
\file Animation.h
\author Camilo Diaz
\e-mail c.diaz@digipen.edu
\date 9/24/2015
\Brief Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
*/
/* End Header *******************************************************************/
#pragma once
class Animation
{
public:
	Animation();
	Animation(float time);
	~Animation();
	
	//Total time of the animation
	float time;

	//Current time of the animation
	float accumulator;

	// Delta ticks between to frames
	float counter;

private:
	
};

