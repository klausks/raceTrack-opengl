#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <map>
#include "Bspline.h"

using namespace std;

class ObjWriter
{
	public:
        ObjWriter(Bspline* bSpline, string path);

        Bspline* bSpline;
        string path;
        string objFile;
        string materialFile;
        const string MATERIAL_NAME = "trackMtl";
        const string GROUP_NAME = "track";
        const string TEXTURE_FILE = "track.jpg";
        float xOffset;
        float zOffset;

        void write();

	private:
        float GLOBAL_SCALE = 0.05f;
        float HEIGHT_SCALE = 0.10f;

        void writeMtlFile(string filePath);
        void writeObjFile(string filePath);
};
