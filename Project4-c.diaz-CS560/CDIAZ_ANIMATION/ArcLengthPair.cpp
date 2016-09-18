#include "ArcLengthPair.h"


ArcLengthPair::ArcLengthPair() :Position(),ParametricValue(0), ArchLength(0)
{
}

ArcLengthPair::ArcLengthPair( Vector3 position, float parametricValue, float arcLength) :
			   Position(position),ParametricValue(parametricValue), ArchLength(arcLength)
{

}

ArcLengthPair::ArcLengthPair(float parametricValue, float arcLength) :Position(),
				ParametricValue(parametricValue), ArchLength(arcLength)
{

}

ArcLengthPair::~ArcLengthPair()
{
}

void ArcLengthPair::SetParametricValue(float parametricValue)
{
	ParametricValue = parametricValue;
}

float ArcLengthPair::GetParametricValue()
{
	return ParametricValue;
}

void ArcLengthPair::SetArchLength(float arcLength)
{
	ArchLength = arcLength;
}

float ArcLengthPair::GetArcLength()
{
	return ArchLength;
}

void ArcLengthPair::SetPosition(Vector3 position)
{
	Position = position;
}

Vector3 ArcLengthPair::GetPosition()
{
	return Position;
}