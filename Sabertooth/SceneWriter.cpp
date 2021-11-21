#include "SceneWriter.h"

SceneWriter::SceneWriter(ObjWriter* objWriter, string targetSceneFolder, string carObjFilePath)
{
    this->objWriter = objWriter;
    this->targetSceneFolder = targetSceneFolder;
    this->curveFile = targetSceneFolder + "curve.txt";
    this->trackObjFile = objWriter->OBJ_FILE;
    this->carObjFilePath = carObjFilePath;
}

void SceneWriter::write()
{
    ofstream sceneFile(targetSceneFolder);
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
        curveFile << curvePoint.x * GLOBAL_SCALE << " " << curvePoint.y * HEIGHT_SCALE << " " << curvePoint.z * GLOBAL_SCALE << endl;
    }
    curveFile.close();
}