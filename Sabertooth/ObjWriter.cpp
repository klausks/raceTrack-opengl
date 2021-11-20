#include "ObjWriter.h"

#pragma once


void ObjWriter::write() {
    writeCurveFile(CURVE_FILE);
    writeMtlFile(MATERIAL_FILE);
    writeObjFile(OBJ_FILE);
}

void ObjWriter::writeCurveFile(string filePath)
{
    ofstream curveFile(filePath);
    vector<glm::vec3> curvePoints = this->bSpline->curvePoints;

    for (glm::vec3 curvePoint : curvePoints)
    {
        curveFile << curvePoint.x * GLOBAL_SCALE << " " << curvePoint.y * HEIGHT_SCALE << " " << curvePoint.z * GLOBAL_SCALE << endl;
    }
    curveFile.close();
}

void ObjWriter::writeMtlFile(string filePath)
{
    ofstream mtlFile(filePath);
    mtlFile << "newmtl " << MATERIAL_NAME << endl;
    mtlFile << "Ka 0.7 0.7 0.7" << endl;
    mtlFile << "Kd 0.9 0.9 0.9" << endl;
    mtlFile << "Ks 1.0 1.0 1.0" << endl;
    mtlFile << "Ns 64.0" << endl;
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

    for (glm::vec3 internalCurvePoint : internalCurvePoints) {
        obj << "v " << (internalCurvePoint.x * GLOBAL_SCALE) << " " << (internalCurvePoint.z * HEIGHT_SCALE) << " " << (internalCurvePoint.y *
            GLOBAL_SCALE)
            << endl;
    }

    vector<glm::vec3> externalCurvePoints = this->bSpline->externalCurvePoints;

    for (glm::vec3 externalCurvePoint : externalCurvePoints) {
        obj << "v " << (externalCurvePoint.x * GLOBAL_SCALE) << " " << (externalCurvePoint.z * HEIGHT_SCALE) << " " << (externalCurvePoint.y *
            GLOBAL_SCALE)
            << endl;
    }

    int noOfCurvePoints = this->bSpline->curvePoints.size();

    for (int i = 1; i < noOfCurvePoints; i++) {
        obj << "f " << i << "/1/1 " << (i + 1) << "/2/1 " << i + noOfCurvePoints << "/4/1" << endl;
        obj << "f " << i + noOfCurvePoints << "/4/1 " << (i + 1) << "/2/1 " << i + 1 + noOfCurvePoints << "/3/1" << endl;
    }

    obj.close();
}



