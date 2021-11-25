#pragma once

#define _USE_MATH_DEFINES
#include <vector>
#include <glm\glm.hpp>
#include <math.h>
#include <GL/glew.h>

using namespace std;

class Bspline {
private:
	const GLfloat CURVE_RESOLUTION = 0.03f;
	const GLfloat CURVE_DISTANCE = 15.0f;

	void updateCurve();
	void updateInAndExCurves();
	glm::vec3 calculateCurvePoint(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, float t);

public:
	Bspline() {};
	vector<glm::vec3> curvePoints, internalCurvePoints, externalCurvePoints;
	vector<glm::vec3> controlPoints;

	// vector<float> cPoints, cvPoints, inPoints, exPoints;
	void insertControlPoint(glm::vec3 point);

	void clear()
	{
		curvePoints.clear();
		internalCurvePoints.clear();
		externalCurvePoints.clear();
		controlPoints.clear();
	}
};