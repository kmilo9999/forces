/* Start Header *****************************************************************/
/*!
\file Curve.h
\author Camilo Diaz
\e-mail c.diaz@digipen.edu
\date 9/24/2015
\Brief Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
*/
/* End Header *******************************************************************/

#pragma once
#include <GL/glew.h>
#include <vector>
#include "Math\Vector3.h"
#include "Math\Transformation.h"
#include "Math\Math.h"
#include "ArcLengthPair.h"
#include "CurvePointPair.h"
#include <fstream>
#include "Math/Interpolation/CRSpline.h"

#define EPSILON_ERROR 0.01f
#define GAMA_ERROR  0.0002f
#define DELTA_ERROR 0.0000001f
#define DELTA 300.f

using namespace std;
class Curve
{
public:
	Curve();
	Curve(vector<Vector3>);
	~Curve();

	void InitCurve(int index);
	void SetCurvePoints(vector<Vector3>);
	vector<Vector3> GetCurvePoints();

	void Render();
	Transformation transformation;

	int GetClosestIndexOfParameterValue(float parameterValue);
	int GetLowEntryIndexOfParameterValue(float parameterValue);
	float GetArcLengthByParameterValue(float parameterValue);
	float GetParameterValueByArchLength(float archLength);
	
	vector<ArcLengthPair> GetArcLengthTable();
	void SetArcLengthTable(vector<ArcLengthPair> arcLengthTable);

	vector<CurvePointPair> GetCurvePairPoints();
	void SetCurvePointTable(vector<CurvePointPair> curvePointTable);

	float BisectionAlgorithm(float arcLength);
	int BinarySearchOfParametricValue(float archLength);
	int BinarySearchOfIndexParametricValue(float archLength);
	void SetArcLengtValueInIndex(int index, float arcLength);
	void SetParametricValueInIndex(int index, float parametricValue);

private:

	void InitArcLengthTable();
	void CalculateArcLength(float Ua, Vector3& P1, float Ub, Vector3& P2, float Len);
	vector<CurvePointPair> CurvePairPoints;
	vector<Vector3> CurvePoints;
	vector<ArcLengthPair> ArcLengthTable;
	vector<Vector3> ControlPoints;
	vector<float> TestSegment;
	GLuint vao;
	GLuint vbo;
	ofstream Myfile;
	CRSpline spline;
	

};



float BisectionAlgorithm( float arcLength);
float Ease(float t, float k1, float k2);
float EaseParabolic(float t, float t1, float t2);
float EaseSpeed(float t, float t1, float t2);