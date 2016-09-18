#include "../Vector3.h"
#include <vector>

class CRSpline
{
public:

	// Constructors and destructor
	CRSpline();
	CRSpline(const CRSpline&);
	~CRSpline();

	// Operations
	void AddSplinePoint(const Vector3& v);
	Vector3 GetInterpolatedSplinePoint(float t);   // t = 0...1; 0=vp[0] ... 1=vp[max]
	int GetNumPoints();
	Vector3& GetNthPoint(int n);

	// Static method for computing the Catmull-Rom parametric equation
	// given a time (t) and a vector quadruple (p1,p2,p3,p4).
	static Vector3 Eq(float t, const Vector3& p1, const Vector3& p2,
		const Vector3& p3, const Vector3& p4);

private:
	std::vector<Vector3> vp;
	float delta_t;
};