/* Start Header *****************************************************************/
/*!
\file Animation.cpp
\author Camilo Diaz
\e-mail c.diaz@digipen.edu
\date 9/24/2015
\Brief Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
*/
/* End Header *******************************************************************/

#include "Animation.h"


Animation::Animation() :time(0.0f), accumulator(0.0f), counter(0.0f)
{
}
Animation::Animation(float t) : time(t), accumulator(0.0f), counter(0.0f)
{

}

Animation::~Animation()
{
}
