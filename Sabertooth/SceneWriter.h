#pragma once
string path = "../Exemplo21/";
string BSLINE = path + "curva.txt";
string OBJ_FILE = path + "pista.obj";
string MATERIAL_FILE = path + "pista.mtl";
string MATERIAL_NAME = "NOME_DO_MATERIAL_AQUI";
string GROUP_NAME = "GRUPO";
string TEXTURE_FILE = "pista.jpg";

void to_obj() {
    ofstream material(MATERIAL_FILE);
    material << "newmtl " << MATERIAL_NAME << endl;
    material << "Ka 0.7 0.7 0.7" << endl;
    material << "Kd 0.9 0.9 0.9" << endl;
    material << "Ks 1.0 1.0 1.0" << endl;
    material << "Ns 64.0" << endl;
    material << "map_Kd " << TEXTURE_FILE << endl;
    material.close();

    ofstream curva(BSLINE);


    float temp = (bspline.size() / ((float)control_points.size() / 3)) * 1;
    for (int i = 0; i < bspline.size() - temp; i += 3) {
        curva << bspline[i] * GLOBAL_SCALE << " " << bspline[i + 2] * HEIGHT_SCALE << " " << bspline[i + 1] * GLOBAL_SCALE << endl;
    }
    curva.close();


    ofstream obj(OBJ_FILE);
    obj << "mtllib " << MATERIAL_FILE << endl;
    obj << "g " << GROUP_NAME << endl;
    obj << "usemtl " << MATERIAL_NAME << endl;

    obj << "vn 0.0 1.0 0.0" << endl;
    obj << "vt 0.0 0.0" << endl;
    obj << "vt 0.0 1.0" << endl;
    obj << "vt 1.0 0.0" << endl;
    obj << "vt 1.0 1.0" << endl;

    int size = bspline_in.size();
    int vertices_size = size / 3;
    for (int i = 0; i < size; i += 3) {
        obj << "v " << (bspline_in[i] * GLOBAL_SCALE) << " " << bspline_in[i + 2] * HEIGHT_SCALE << " " << (bspline_in[i + 1] *
            GLOBAL_SCALE)
            << endl;
    }
    for (int i = 0; i < size; i += 3) {
        obj << "v " << (bspline_ex[i] * GLOBAL_SCALE) << " " << bspline_ex[i + 2] * HEIGHT_SCALE << " " << (bspline_ex[i + 1] *
            GLOBAL_SCALE)
            << endl;
    }

    for (int i = 1; i <= size / 3 - 3; i++) {
        obj << "f " << i << "/1/1 " << (i + 1) << "/2/1 " << i + vertices_size << "/4/1" << endl;
        obj << "f " << i + vertices_size << "/4/1 " << (i + 1) << "/2/1 " << i + 1 + vertices_size << "/3/1" << endl;
    }

    obj.close();
}
