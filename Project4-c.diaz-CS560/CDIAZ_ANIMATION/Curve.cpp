/* Start Header *****************************************************************/
/*!
\file Curve.cpp
\author Camilo Diaz
\e-mail c.diaz@digipen.edu
\date 9/24/2015
\Brief Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
*/
/* End Header *******************************************************************/
#include "Curve.h"


Curve::Curve() :CurvePoints(), transformation(), ControlPoints()
{
	
}

Curve::Curve(vector<Vector3> controlPoints) : ControlPoints(controlPoints)
{

}

Curve::~Curve()
{
}

/*
	Set the initial control Points
*/
void Curve::SetCurvePoints(vector<Vector3> curvePoints)
{
	CurvePoints = curvePoints;
}

vector<Vector3> Curve::GetCurvePoints()
{
	return CurvePoints;
}

/*
	Initialize thw interpolation between the control Points using Carmull Roll Spline
*/
void Curve::InitCurve(int index)
{


	for (unsigned int i = 0; i < ControlPoints.size(); i++)
	{
		spline.AddSplinePoint(ControlPoints[i]);
	}

	for (int i = 0; i < DELTA; i++) {
		float t = (float)i / (float)DELTA;
		TestSegment.push_back(t);
	}
	TestSegment.push_back(1.0f);
	

	InitArcLengthTable();
}

/*
	Render the curve
*/
void Curve::Render()
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glBindVertexArray(vao); 
	glDrawArrays(GL_POINTS, 0, CurvePoints.size());
	glDisable(GL_PROGRAM_POINT_SIZE);
	glBindVertexArray(0);
}

/*
Initialize Arc Length table Using Adaptative approach
*/
void Curve::InitArcLengthTable()
{
	CurvePairPoints.clear();
	Vector3 P0 = spline.GetInterpolatedSplinePoint(0.0f);
	CurvePairPoints.push_back(CurvePointPair(0.0f,P0));
	CurvePoints.push_back(P0);
	ArcLengthTable.push_back(ArcLengthPair(P0,CurvePairPoints[0].GetParameterU(), 0.0f));

	


	while (!TestSegment.empty())
	{
		if (TestSegment.size() == 1)
		{
			TestSegment.erase(TestSegment.begin() + 0);
		}
		else{
			std::vector<float>::iterator it = TestSegment.begin();
			float Ua = *it;
			*it++;
			float Ub = *it;
			Vector3 P1 = spline.GetInterpolatedSplinePoint(Ua);
			Vector3 P2 = spline.GetInterpolatedSplinePoint(Ub);
			float length = Math::Distance(P1, P2);
			CalculateArcLength(Ua, P1, Ub, P2, length);
			TestSegment.erase(TestSegment.begin() + 0);
		}
		
	}

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(CurvePoints[0]) * CurvePoints.size(),
		&CurvePoints[0],
		GL_STATIC_DRAW
		);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	int size = ArcLengthTable.size();
	float normalizeArcLengthValue = ArcLengthTable[size-1].GetArcLength();

}

/*
	Calculate the Arc lenght from a Point P1 to a Point P2
*/
void Curve::CalculateArcLength(float Ua, Vector3& P1, float Ub, Vector3& P2, float Len)
{
	float Um = (Ua + Ub) * 0.5f;
	Vector3 PUm = spline.GetInterpolatedSplinePoint(Um);
	float A = Math::Distance(PUm, P1);
	float B = Math::Distance(P2, PUm);
	float distance = A + B - Len;

	if (fabsf(Ua - Ub) < EPSILON_ERROR && fabsf(distance) < GAMA_ERROR)
	{
		ArcLengthPair arcLength = ArcLengthTable.back();
		float currentArcLength = arcLength.GetArcLength();
		ArcLengthTable.push_back(ArcLengthPair(PUm, Um, currentArcLength + A));
		CurvePairPoints.push_back(CurvePointPair(Um,PUm));
		CurvePoints.push_back(PUm);
		ArcLengthTable.push_back(ArcLengthPair(P2,Ub, currentArcLength + A + B));
		CurvePairPoints.push_back(CurvePointPair(Ub, P2));
		CurvePoints.push_back(P2);
	}
	else{
		
		CalculateArcLength(Ua, P1, Um, PUm, A);
		CalculateArcLength(Um, PUm, Ub, P2, B);
	}
	
}

/*
	Performs binary search on the arc length table to find ht closest index for
	a given Parameter U
*/
int Curve::GetClosestIndexOfParameterValue(float parameterValue)
{

	int min = 0;
	int max = ArcLengthTable.size();
	while (min < max)
	{
		int midPoint = min + ((max - min) / 2);
		if (ArcLengthTable[midPoint].GetParametricValue() < parameterValue)
		{
			if (ArcLengthTable[midPoint + 1].GetParametricValue() < parameterValue)
			{
				min = midPoint + 1;
			}
			else if (ArcLengthTable[midPoint + 1].GetParametricValue() >= parameterValue)
			{
				return midPoint + 1;
			}
		}
		else if (ArcLengthTable[midPoint].GetParametricValue() > parameterValue)
		{
			if (ArcLengthTable[midPoint - 1].GetParametricValue() > parameterValue)
			{
				max = midPoint - 1;
			}
			else if (ArcLengthTable[midPoint - 1].GetParametricValue() <= parameterValue)
			{
				return midPoint - 1;
			}
		}
		else
		{
			return midPoint;
		}
	}

	// it should never reach this point
	return 0;

	
}

/*
	Performs binary search on the arc length table to find ht closest index for
	a given Parameter U
*/
int Curve::GetLowEntryIndexOfParameterValue(float parameterValue)
{
	int min = 0;
	int max = ArcLengthTable.size();
	while (min < max)
	{
		int midPoint = min + ((max - min) / 2);
		if (ArcLengthTable[midPoint].GetParametricValue() == parameterValue)
		{
			return midPoint;
		}
		if (ArcLengthTable[midPoint].GetParametricValue() < parameterValue)
		{
			
			 if (ArcLengthTable[midPoint + 1].GetParametricValue() < parameterValue)
			{
				min = midPoint + 1;
			}
			else if (ArcLengthTable[midPoint + 1].GetParametricValue() >= parameterValue)
			{
				return midPoint + 1;
			}
		}
		else if (ArcLengthTable[midPoint].GetParametricValue() > parameterValue)
		{
			if (ArcLengthTable[midPoint - 1].GetParametricValue() > parameterValue)
			{
				max = midPoint - 1;
			}
			else if (ArcLengthTable[midPoint - 1].GetParametricValue() <= parameterValue)
			{
				return midPoint - 1;
			}
		}
		else
		{
			return midPoint;
		}
	}

	// it should never reach this point
	return 0;
}

/*
	Get the closest index to a parameter U . This technique was used before using
	adaptative aproach
*/
float Curve::GetArcLengthByParameterValue(float parameterValue)
{
	float deltaU = 1.0f / DELTA;

	unsigned int index = (int)(parameterValue / deltaU);
	
	ArcLengthPair arcLength = ArcLengthTable[index];
	
	float K = (parameterValue - arcLength.GetParametricValue()) / deltaU;

	if (index + 1 >= ArcLengthTable.size())
	{
		return  ArcLengthTable[index].GetArcLength();
	}

	return arcLength.GetArcLength() + K * (ArcLengthTable[index + 1].GetArcLength() - arcLength.GetArcLength());
}

/*
	Returns a paramter U to a given arc length.This technique was used before using
	adaptative aproach
*/
float Curve::GetParameterValueByArchLength(float archLength)
{
	float deltaU = 1.0f / DELTA;
	if (archLength < 0.0f)
	{
		archLength = 0.0f;
	}
	else if (archLength > 1.0f)
	{
		archLength = 1.0f;
	}

	int lowerIndex = BinarySearchOfParametricValue(archLength);

	if (lowerIndex + 1.0f >= ArcLengthTable.size())
	{
		return  ArcLengthTable[lowerIndex].GetArcLength();
	}


	float indexParameterValue = ArcLengthTable[lowerIndex].GetParametricValue();
	float indexArcLength = ArcLengthTable[lowerIndex].GetArcLength();


	
	float indexPlusOneArcLength = ArcLengthTable[lowerIndex + 1].GetArcLength();

	float K = deltaU / (indexPlusOneArcLength - indexArcLength);

	return indexParameterValue + K * (archLength - indexArcLength);

}

// Perfoms Binary search
/*
	Perfoms binary search to find  a parameter U for a given arc length 
*/
int Curve::BinarySearchOfParametricValue( float archLength)
{
	int min = 0;
	int max = ArcLengthTable.size();
	while (min < max)
	{
		int midPoint = min + ((max - min) / 2);
		if (ArcLengthTable[midPoint].GetArcLength() < archLength)
		{
			if(ArcLengthTable[midPoint + 1].GetArcLength() < archLength)
			{
				min = midPoint + 1;
			}
			else if (ArcLengthTable[midPoint + 1].GetArcLength() >= archLength)
			{
				return midPoint + 1;
			}
		}	
		else if (ArcLengthTable[midPoint].GetArcLength() > archLength)
		{
			if (ArcLengthTable[midPoint-1].GetArcLength() > archLength)
			{
				max = midPoint - 1;
			}
			else if (ArcLengthTable[midPoint - 1].GetArcLength() <= archLength)
			{
				return midPoint - 1;
			}							
		}
		else
		{
			return midPoint;
		}
	}
	
	// it should never reach this point
	return 0;
}

/*
	Perfoms BisectionAlgorithm to find a parameter U for a given Arc Length S
*/
float Curve::BisectionAlgorithm(float S)
{
	int iterations = 40;
	float Ua = 0.0f;
	float Ub = 1.0f;
	float Sm = 0.0f;
	float Um = 0;
	int count = 0;
	float difference = 0.0f;
	do
	{
		if (count == iterations)
		{
			break;
		}


		Um = (Ua + Ub) * 0.5f;
		Sm = GetArcLengthByParameterValue(Um);
		if (Sm < S)
		{
			Ua = Um;
		}
		else
		{
			Ub = Um;
		}
		count++;
		difference = abs(Sm - S);
	} while ( difference > 0.0000001);

	return Um;
}


/*
	Perfoms Easy In Out using sin approach
*/
float Ease(float t, float k1, float k2)
{
	float PI = 3.141516f;
	float t1=0.0f, t2=0.0f;
	float f, s;
	f = k1 * 2.0f / PI + k2 - k1 + (1.0f - k2) * 2.0f / PI;
	if (t < k1)
	{
		s = k1 * (2.0f / PI) * (sin((t / k1)*PI / 2.0f - PI / 2.0f) + 1.0f);
	
	}
	else if (t <k2)
	{
		s = (2.0f * k1 / PI + t - k1);
	}
	else
	{
		s = 2.0f * k1 / PI + k2 - k1 + ((1.0f - k2)*(2.0f / PI)) * sinf(((t - k2) / (1.0f - k2)) * PI / 2);
	}

	return (s / f);
}

/*
	Perfoms Easy In Out using Parabolic approach
*/
float EaseParabolic(float t, float t1, float t2)
{
	float v0 = 2 / (1 - t1 + t2);

	float s = 0.0f;

	if (t < t1)
	{
		s = v0/(2*t1) * powf(t,2);

	}
	else if (t < t2)
	{
		s = v0*(t - (t1 / 2));
	}
	else
	{
		s = (v0*(t - t2) / 2 * (1 - t2)) * (2 - t - t2) + v0 * (t2 - (t1 / 2));
	}

	return s;
}

/*
	Finds the speed of the a particle at time T using Parabolci Easy In - Out
*/
float EaseSpeed(float t, float t1, float t2)
{
	float speed = 0.0f;
	float v0 = 2 / (1 - t1 + t2);
	
	if (t > 1.0f)
	{
		t = 1.0f;
	}

	if (t < t1)
	{
		speed = v0 * t /  t1;

	}
	else if (t < t2)
	{
		speed = v0;
	}
	else
	{
		speed = ((v0*(1-t))/ (1-t2));
	}

	return speed;
}

/*
 returns the arc length table of this curve
*/
vector<ArcLengthPair> Curve::GetArcLengthTable()
{
	return ArcLengthTable;
}

vector<CurvePointPair> Curve::GetCurvePairPoints()
{
	return CurvePairPoints;
}

void Curve::SetCurvePointTable(vector<CurvePointPair> curvePointTable)
{
	CurvePairPoints = curvePointTable;
}

void Curve::SetArcLengthTable(vector<ArcLengthPair> arcLengthTable)
{
	ArcLengthTable = arcLengthTable;
}

void Curve::SetArcLengtValueInIndex(int index, float arcLength)
{
	ArcLengthTable[index].SetArchLength(arcLength);
}

void Curve::SetParametricValueInIndex(int index, float parametricValue)
{
	ArcLengthTable[index].SetParametricValue(parametricValue);
}


int Curve::BinarySearchOfIndexParametricValue(float archLength)
{
	int min = 0;
	int max = ArcLengthTable.size();
	while (min < max)
	{
		int midPoint = min + ((max - min) / 2);
		if (ArcLengthTable[midPoint].GetArcLength() < archLength)
		{
			if (ArcLengthTable[midPoint + 1].GetArcLength() < archLength)
			{
				min = midPoint + 1;
			}
			else if (ArcLengthTable[midPoint + 1].GetArcLength() >= archLength)
			{
				return midPoint + 1;
			}
		}
		else if (ArcLengthTable[midPoint].GetArcLength() > archLength)
		{
			if (ArcLengthTable[midPoint - 1].GetArcLength() > archLength)
			{
				max = midPoint - 1;
			}
			else if (ArcLengthTable[midPoint - 1].GetArcLength() <= archLength)
			{
				return midPoint - 1;
			}
		}
		else
		{
			return midPoint;
		}
	}

	// it should never reach this point
	return 0;
}

