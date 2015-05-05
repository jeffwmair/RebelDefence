//
//  MeshLoaderJwm.h
//  cps511a03
//
//  Created by Jeff Mair on 2013-11-30.
//  Copyright (c) 2013 Jeff Mair. All rights reserved.
//

#ifndef __cps511a03__MeshLoaderJwm__
#define __cps511a03__MeshLoaderJwm__

#include <iostream>
#include <vector>
#include "Mesh.h"
#include "Vertex.h"
#include "Slice.h"
#include "VertexQuad.h"

class MeshLoaderJwm {
private:
    
    int highestVertexId;
    std::vector<Slice * > _slices;
    std::vector<Quad * > _quads;
    // array of vectors of Quad pointers - indexed by vertex id
    std::vector<VertexQuad *> _vertexQuads;
    std::vector<Vertex3 * > _verticies;
    
    void addVertexToIndexes(Mesh * m, Vertex3 * v, Quad * q);
    void addObjectSlice(Mesh * m, Slice * slice);
    void addSliceVerticies(Mesh * m, std::vector<Vertex3*> newVerts);
	VertexQuad * getVertexQuadById(int id);
    bool hasVerticeWithId(int id);
public:
    Mesh * loadMesh(std::string filename);
};
#endif /* defined(__cps511a03__MeshLoaderJwm__) */
