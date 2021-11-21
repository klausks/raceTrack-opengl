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
		ObjWriter(Bspline* bSpline, string path)
        {
            this->bSpline = bSpline;
            this->path = path;
            this->OBJ_FILE = path + "track.obj";
            this->MATERIAL_FILE = path + "track.mtl";
        }

        Bspline* bSpline;
        string path;
        string OBJ_FILE;
        string MATERIAL_FILE;
        string MATERIAL_NAME = "trackMtl";
        string GROUP_NAME = "track";
        string TEXTURE_FILE = "track.jpg";

        void write();

	private:
        float GLOBAL_SCALE = 0.01f;
        float HEIGHT_SCALE = 0.01f;
        void writeMtlFile(string filePath);
        void writeObjFile(string filePath);
};
