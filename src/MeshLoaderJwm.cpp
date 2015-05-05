//
//  MeshLoaderJwm.cpp
//  cps511a03
//
//  Created by Jeff Mair on 2013-11-30.
//  Copyright (c) 2013 Jeff Mair. All rights reserved.
//

#include "MeshLoaderJwm.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

Mesh * MeshLoaderJwm::loadMesh(std::string fname) {
    
    _quads.clear();
    _slices.clear();
    _vertexQuads.clear();
    _verticies.clear();
    
    Mesh * newMesh = new Mesh();
    
    // read the slices from the file
    std::vector<Slice*> slices;
    Slice* s = new Slice();
    std::string line;
    std::ifstream fileIn (fname.c_str());
    if (fileIn.is_open())
    {
        while ( getline (fileIn, line) )
        {
            if (line == "") {
                //                delete s;
                s = new Slice();
                slices.push_back(s);
            }
            else {
                std::stringstream ss(line);
                std::string Mesh;
                float x, y, z;
                int i = 0;
                while(getline(ss, Mesh, ' ')) {
                    float val = atof(Mesh.substr(2, std::string::npos).c_str());
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
                
                Vertex3 * v = new Vertex3(x, y, z);
                s->verticies.push_back(v);
                
            }
        }
        //        delete s;
        fileIn.close();
    }
    
    for(int i = 0; i < slices.size(); i++) {
        addSliceVerticies(newMesh, slices[i]->verticies);
    }
    
    newMesh->setTotalNumVerticies(_quads.size() * NUM_VERTICIES_PER_QUAD);
    return newMesh;
    
}


void MeshLoaderJwm::addSliceVerticies(Mesh * m, std::vector<Vertex3*> newVerts) {
    Slice *slice = new Slice();
    for(vector<Vertex3*>::iterator itr = newVerts.begin(); itr != newVerts.end(); ++itr) {
        (*itr)->setId(++highestVertexId);
        slice->verticies.push_back(*itr);
    }
    
    /* get a reference to the last slice in this current one */
    if (_slices.size() > 0) {
        slice->previous = _slices.back();
        slice->hasPrevious = true;
    }
    
    _slices.push_back(slice);
    addObjectSlice(m, slice);
    
}

void MeshLoaderJwm::addVertexToIndexes(Mesh * m, Vertex3* v, Quad * q) {
    if (!hasVerticeWithId(v->getId())) {
        vector<Quad *> newVertexQuadList;
        newVertexQuadList.push_back(q);
        VertexQuad *vq = new VertexQuad;
        vq->vertexId = v->getId();
        vq->quads = newVertexQuadList;
        _vertexQuads.push_back(vq);
    }
    else {
        VertexQuad * vq = getVertexQuadById(v->getId());
        vq->quads.push_back(q);
    }
    
    if (!hasVerticeWithId(v->getId())) {
        _verticies.push_back(v);
    }
    
}

VertexQuad * MeshLoaderJwm::getVertexQuadById(int id) {
    for(int i = 0; i < _vertexQuads.size(); i++) {
        if (_vertexQuads[i]->vertexId == id) {
            return _vertexQuads[i];
        }
    }
}

void MeshLoaderJwm::addObjectSlice(Mesh * m, Slice *slice) {
    
    if (slice->hasPrevious) {
        // we can make quads (with normal vectors) between this slice and the previous one
        for(int i = 0; i < slice->verticies.size(); i++) {
            // need to pass by one vertex to create the quad
            if (i > 0) {
                Vertex3 * v1 = slice->verticies[i];
                Vertex3 * v2 = slice->previous->verticies[i];
                Vertex3 * v3 = slice->previous->verticies[i-1];
                Vertex3 * v4 = slice->verticies[i-1];
                
                Quad * q = new Quad(v1, v2, v3, v4);
                
                /* we only need this list for iterating to calculate normal vectors
                 at the bottom of this routine */
                /* if the total number of vertices is equal to this v.id, then this v won't
                 have a corresponding vector of quads yet, so create it */
                addVertexToIndexes(m, v1, q);
                addVertexToIndexes(m, v2, q);
                addVertexToIndexes(m, v3, q);
                addVertexToIndexes(m, v4, q);
                
                /* and finally, keep this quad in the main quads list */
                _quads.push_back(q);
            }
        }
        
        /* and calculate the vector normals of every vertice using the average of the adjacent quad normals */
        for(int i = 0; i < _verticies.size(); i++) {
            Vertex3 * v = _verticies[i];
            VertexQuad * vq = getVertexQuadById(v->getId());
            GLfloat sumNormalX = 0.0f;
            GLfloat sumNormalY = 0.0f;
            GLfloat sumNormalZ = 0.0f;
            int quadCounter = 0;
            
            for(int j = 0; j < vq->quads.size(); j++) {
                sumNormalX += vq->quads[j]->getNormal()[0];
                sumNormalY += vq->quads[j]->getNormal()[1];
                sumNormalZ += vq->quads[j]->getNormal()[2];
                quadCounter++;
            }
            
            GLfloat normalX = sumNormalX / quadCounter;
            GLfloat normalY = sumNormalY / quadCounter;
            GLfloat normalZ = sumNormalZ / quadCounter;
            /* tweaks for A3:
             - lighting looked backward compared to GlutSolidCube, so I reversed the normals
             - lighting was also too dark, so I added a normal multiplier
             */
#define NORMAL_MULT 2
            v->setNormal(-normalX*NORMAL_MULT, -normalY*NORMAL_MULT, -normalZ*NORMAL_MULT);
        }
        
        /* put all the verticies and normals in big arrays for VAO-based rendering */
        int pointsPerQuad = NUM_VERTICIES_PER_QUAD * NUM_POINTS_PER_VERTEX;
        int allVerticiesSize = _quads.size() * pointsPerQuad;
        m->_verticiesAll = new GLfloat[allVerticiesSize];
        m->_normalsAll = new GLfloat[allVerticiesSize];
        
        int vertexIndex = 0;
        
        // need to put verticies in the VAO ordered by quad so that the faces are drawn properly
        
        //        for(int i = 0; i < _slices.size(); i++) {
        //            Slice * s = _slices[i];
        //            vector<Vertex3*> verticies = s->verticies;
        //            for(vector<Vertex3*>::iterator itr = verticies.begin(); itr != verticies.end(); ++itr) {
        //                Vertex3 * v = *itr;
        //                _normalsAll[vertexIndex] = v->getNormal()[0];
        //                _verticiesAll[vertexIndex] = v->getX();
        //                vertexIndex++;
        //                _normalsAll[vertexIndex] = v->getNormal()[1];
        //                _verticiesAll[vertexIndex] = v->getY();
        //                vertexIndex++;
        //                _normalsAll[vertexIndex] = v->getNormal()[2];
        //                _verticiesAll[vertexIndex] = v->getZ();
        //                vertexIndex++;
        //            }
        //
        //        }
        
        for(int i = 0; i < _quads.size(); i++) {
            Quad * q = _quads[i];
            for(int j = 0; j < 4; j++) {
                Vertex3 * v = q->verticies[j];
                m->_normalsAll[vertexIndex] = v->getNormal()[0];
                m->_verticiesAll[vertexIndex] = v->getX();
                vertexIndex++;
                m->_normalsAll[vertexIndex] = v->getNormal()[1];
                m->_verticiesAll[vertexIndex] = v->getY();
                vertexIndex++;
                m->_normalsAll[vertexIndex] = v->getNormal()[2];
                m->_verticiesAll[vertexIndex] = v->getZ();
                vertexIndex++;
                
            }
        }
    }
}

bool MeshLoaderJwm::hasVerticeWithId(int id) {
    for(int i = 0; i < _verticies.size(); i++) {
        if (_verticies[i]->getId() == id) {
            return true;
        }
    }
    
    return false;
}

