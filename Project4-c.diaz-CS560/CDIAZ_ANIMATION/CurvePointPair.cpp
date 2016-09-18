#include "CurvePointPair.h"


CurvePointPair::CurvePointPair() :ParameterU(0.0f), Point(Vector3())
{

}

CurvePointPair::CurvePointPair(float parameterU, Vector3 point) :
						ParameterU(parameterU), Point(point)
{

}

CurvePointPair::~CurvePointPair()
{
}

void CurvePointPair::SetParamterU(float parameterU)
{
	ParameterU = parameterU;
}

float CurvePointPair::GetParameterU()
{
	return ParameterU;
}

void CurvePointPair::SetPoint(Vector3 point)
{
	Point = point;
}

Vector3 CurvePointPair::GetPoint()
{
	return Point;
}