#include "SceneWriter.h"

SceneWriter::SceneWriter(ObjWriter* objWriter, string targetSceneFolder, string carObjFilePath)
{
    this->objWriter = objWriter;
    this->sceneFile = targetSceneFolder + "scene.txt";
    this->curveFile = targetSceneFolder + "curve.txt";
    this->trackObjFile = objWriter->OBJ_FILE;
    this->carObjFilePath = carObjFilePath;
}

void SceneWriter::write()
{
    ofstream sceneFile(sceneFile);
    sceneFile << "obj Car " << carObjFilePath << endl;
    sceneFile << "obj Track " << trackObjFile << endl;
    sceneFile << "curve " << curveFile << endl;
    sceneFile.close();
    writeCurveFile(curveFile);
}

void SceneWriter::writeCurveFile(string filePath)
{
    ofstream curveFile(filePath);
    vector<glm::vec3> curvePoints = this->objWriter->bSpline->curvePoints;

    for (glm::vec3 curvePoint : curvePoints)
    {
        curveFile << (curvePoint.x + objWriter->xOffset) * GLOBAL_SCALE << " " << curvePoint.z * HEIGHT_SCALE << " " << (curvePoint.y + objWriter->zOffset) * GLOBAL_SCALE << endl;
    }
    curveFile.close();
}