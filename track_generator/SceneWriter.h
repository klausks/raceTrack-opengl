#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <map>
#include "Bspline.h"
#include "ObjWriter.h"

using namespace std;

class SceneWriter
{
public:
    SceneWriter(ObjWriter* objWriter, string targetSceneFolder, string objFilePath);
    void write();

private:
    void writeCurveFile(string filePath);
    string sceneFile;
    string curveFile;
    string carObjFilePath;
    string trackObjFile;
    ObjWriter* objWriter;

    float GLOBAL_SCALE = 0.3f;
    float HEIGHT_SCALE = 0.3f;
};
