#include "ObjWriter.h"

#pragma once

ObjWriter::ObjWriter(Bspline* bSpline, string path)
{
    this->bSpline = bSpline;
    this->xOffset = -bSpline->curvePoints[0].x;
    this->zOffset = -bSpline->curvePoints[0].y;
    this->path = path;
    this->objFile = path + "track.obj";
    this->materialFile = path + "track.mtl";
}

void ObjWriter::write()
{
    writeMtlFile(materialFile);
    writeObjFile(objFile);
}

void ObjWriter::writeMtlFile(string filePath)
{
    ofstream mtlFile(filePath);
    mtlFile << "newmtl " << MATERIAL_NAME << endl;
    mtlFile << "Ka 1.0 1.0 1.0" << endl;
    mtlFile << "Kd 0.5 0.5 0.5" << endl;
    mtlFile << "Ks 0.1 0.1 0.1" << endl;
    mtlFile << "Ns 10.0" << endl;
    mtlFile << "map_Kd " << TEXTURE_FILE << endl;
    mtlFile.close();
}

void ObjWriter::writeObjFile(string filePath)
{
    ofstream obj(filePath);
    obj << "mtllib " << "track.mtl" << endl;
    obj << "g " << GROUP_NAME << endl;
    obj << "usemtl " << MATERIAL_NAME << endl;

    obj << "vn 0.0 1.0 0.0" << endl;
    obj << "vt 0.0 0.0" << endl;
    obj << "vt 0.0 1.0" << endl;
    obj << "vt 1.0 0.0" << endl;
    obj << "vt 1.0 1.0" << endl;

    vector<glm::vec3> internalCurvePoints = this->bSpline->internalCurvePoints;
    for (glm::vec3 internalCurvePoint : internalCurvePoints)
    {
        obj << "v " << ((internalCurvePoint.x + xOffset) * GLOBAL_SCALE) << " " << (internalCurvePoint.z * HEIGHT_SCALE) << " " << ((internalCurvePoint.y + zOffset) *
            GLOBAL_SCALE)
            << endl;
    }

    vector<glm::vec3> externalCurvePoints = this->bSpline->externalCurvePoints;
    for (glm::vec3 externalCurvePoint : externalCurvePoints)
    {
        obj << "v " << ((externalCurvePoint.x + xOffset) * GLOBAL_SCALE) << " " << (externalCurvePoint.z * HEIGHT_SCALE) << " " << ((externalCurvePoint.y + zOffset) *
            GLOBAL_SCALE)
            << endl;
    }

    int noOfCurvePoints = this->bSpline->curvePoints.size();
    for (int i = 1; i < noOfCurvePoints; i++)
    {
        obj << "f " << i << "/1/1 " << (i + 1) << "/2/1 " << i + noOfCurvePoints << "/4/1" << endl;
        obj << "f " << i + noOfCurvePoints << "/4/1 " << (i + 1) << "/2/1 " << i + 1 + noOfCurvePoints << "/3/1" << endl;
    }
    obj.close();
}



