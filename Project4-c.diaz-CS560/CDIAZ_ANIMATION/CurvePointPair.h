#pragma once
#include "Math/Vector3.h"
class CurvePointPair
{
public:
	CurvePointPair();
	CurvePointPair(float parameterU, Vector3 point);
	~CurvePointPair();

	void SetParamterU(float parameterU);
	float GetParameterU();

	void SetPoint(Vector3 point);
	Vector3 GetPoint();

private:

	float ParameterU;
	Vector3 Point;
};

