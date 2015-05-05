//
//  MeshLoader.cpp
//  cps511a03
//
//  Created by Jeff Mair on 2013-11-30.
//  Copyright (c) 2013 Jeff Mair. All rights reserved.
//

#include "MeshLoaderObj.h"

using namespace std;

Mesh * MeshLoaderObj::loadMesh(std::string fname) {
    std::string line;
    std::ifstream fileIn (fname.c_str());
    int normalCount = 0;
    int vertexCount = 0;
    vector<float> verticePointsInOrder;
    vector<float> normalPointsInOrder;
    if (fileIn.is_open())
    {
        while ( getline (fileIn, line) )
        {
            if (line == "") {
                //                delete s;
//                s = new Slice();
//                slices.push_back(s);
            }
            else {

                string prefix = line.substr(0,2);
                if (prefix == "v " || prefix == "vn") {
                    
                    bool isVertex = (prefix == "v ");
                    
                    int dataStartIndex = (isVertex) ? 2 : 3;
                    std::stringstream ss(line.substr(dataStartIndex));
                    string points;
                    float x, y, z;
                    int i = 0;
                    while(getline(ss, points, ' ')) {
                        float val = atof(points.c_str());
                        if (i == 0) {
                            x = val;
                        }
                        else if (i == 1) {
                            y = val;
                        }
                        else if (i == 2) {
                            z = val;
                        }
                        i++;
                    }
                    
                    if (isVertex) {
                        verticePointsInOrder.push_back(x);
                        verticePointsInOrder.push_back(y);
                        verticePointsInOrder.push_back(z);
                    }
                    else {
                        normalPointsInOrder.push_back(x);
                        normalPointsInOrder.push_back(y);
                        normalPointsInOrder.push_back(z);
                    }
                    
                    
//                    string v = line.substr(2);
//                    cout << "vertex: " << v << endl;
//                    vertexCount++;
//                }
//                else if (prefix == "vn") {
//                    string n = line.substr(3);
//                    cout << "normal: " << n << endl;
//                    normalCount++;
                }
//                std::string Mesh;
//                float x, y, z;
//                int i = 0;
//                while(getline(ss, Mesh, ' ')) {
//                    float val = atof(Mesh.substr(2, std::string::npos).c_str());
//                    if (i == 0) {
//                        x = val;
//                    }
//                    else if (i == 1) {
//                        y = val;
//                    }
//                    else if (i == 2) {
//                        z = val;
//                    }
//                    i++;
//                }
                

                
            }
        }
        //        delete s;
        fileIn.close();
        
        Mesh * newMesh = new Mesh();
        
        cout << "NumVertices:" << vertexCount << endl;
        cout << "NumNormals:" << normalCount << endl;
        
    }

}