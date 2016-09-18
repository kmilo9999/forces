#pragma once
#include "Math\Vector3.h"
class ArcLengthPair
{
public:
	ArcLengthPair();
	ArcLengthPair(Vector3 position, float parametricValue, float arcLength);
	ArcLengthPair( float parametricValue, float arcLength);
	~ArcLengthPair();

	void SetParametricValue(float);
	float GetParametricValue();

	void SetArchLength(float);
	float GetArcLength();

	void SetPosition(Vector3 position);
	Vector3 GetPosition();

private:
	float ParametricValue;
	float ArchLength;
	Vector3 Position;
};

