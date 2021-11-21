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
    SceneWriter(ObjWriter* objWriter, string targetSceneFolder, string carObjFilePath);
    void write();

private:
    void writeCurveFile(string filePath);
    string targetSceneFolder;
    string curveFile;
    string carObjFilePath;
    string trackObjFile;
    ObjWriter* objWriter;

    float GLOBAL_SCALE = 0.01f;
    float HEIGHT_SCALE = 0.01f;
};
