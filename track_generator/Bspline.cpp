#include "Bspline.h"

void Bspline::insertControlPoint(glm::vec3 point)
{
	controlPoints.push_back(point);
    update();
}

void Bspline::update()
{
    updateCurve();
    updateInAndExCurves();
}

void Bspline::updateCurve()
{
    curvePoints.clear();
    int size = controlPoints.size();

    for (int i = 0; i < size; i++)
    {
        glm::vec3 p1 = controlPoints[i];
        glm::vec3 p2 = controlPoints[(i + 1) % size];
        glm::vec3 p3 = controlPoints[(i + 2) % size];
        glm::vec3 p4 = controlPoints[(i + 3) % size];

        for (float t = 0; t < 1; t += CURVE_RESOLUTION)
        {
            glm::vec3 curvePoint = calculateCurvePoint(p1, p2, p3, p4, t);
            curvePoints.push_back(curvePoint);
        }
    }
}

void Bspline::clear()
{
    curvePoints.clear();
    internalCurvePoints.clear();
    externalCurvePoints.clear();
    controlPoints.clear();
}

glm::vec3 Bspline::calculateCurvePoint(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, float t)
{
    float tPow3 = pow(t, 3);
    float tPow2 = pow(t, 2);
    float x =
        (
            (
                (-1 * tPow3 + 3 * tPow2 - 3 * t + 1) * p1.x +
                (+3 * tPow3 - 6 * tPow2 + 0 * t + 4) * p2.x +
                (-3 * tPow3 + 3 * tPow2 + 3 * t + 1) * p3.x +
                (+1 * tPow3 + 0 * tPow2 + 0 * t + 0) * p4.x
                ) / 6.0f
            );

    float y =
        (
            (
                (-1 * tPow3 + 3 * tPow2 - 3 * t + 1) * p1.y +
                (+3 * tPow3 - 6 * tPow2 + 0 * t + 4) * p2.y +
                (-3 * tPow3 + 3 * tPow2 + 3 * t + 1) * p3.y +
                (+1 * tPow3 + 0 * tPow2 + 0 * t + 0) * p4.y
                ) / 6.0f
            );

    float z =
        (
            (
                (-1 * tPow3 + 3 * tPow2 - 3 * t + 1) * p1.z +
                (+3 * tPow3 - 6 * tPow2 + 0 * t + 4) * p2.z +
                (-3 * tPow3 + 3 * tPow2 + 3 * t + 1) * p3.z +
                (+1 * tPow3 + 0 * tPow2 + 0 * t + 0) * p4.z
                ) / 6.0f
            );

    return glm::vec3(x, y, z);
}

void Bspline::updateInAndExCurves()
{
    internalCurvePoints.clear();
    externalCurvePoints.clear();

    int size = curvePoints.size();
    for (int i = 0; i < size; i++)
    {
        float Ax = curvePoints[i].x;
        float Ay = curvePoints[i].y;

        float Bx = curvePoints[(i + 1) % size].x;
        float By = curvePoints[(i + 1) % size].y;

        float w = Bx - Ax;
        float h = By - Ay;
        float a = atan(h / w);
        float internalAngle, externalAngle;
        if (w < 0) {
            internalAngle = a - M_PI / 2;
            externalAngle = a + M_PI / 2;
        }
        else {
            internalAngle = a + M_PI / 2;
            externalAngle = a - M_PI / 2;
        }

        float internalCx = cos(internalAngle) * CURVE_DISTANCE + Ax;
        float internalCy = sin(internalAngle) * CURVE_DISTANCE + Ay;
        internalCurvePoints.push_back(glm::vec3(internalCx, internalCy, curvePoints[i].z));

        float externalCx = cos(externalAngle) * CURVE_DISTANCE + Ax;
        float externalCy = sin(externalAngle) * CURVE_DISTANCE + Ay;
        externalCurvePoints.push_back(glm::vec3(externalCx, externalCy, curvePoints[i].z));
    }
}