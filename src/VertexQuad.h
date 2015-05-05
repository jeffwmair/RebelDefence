//
//  VertexQuad.h
//  cps511a2
//
//  Created by Jeff Mair on 2013-11-02.
//  Copyright (c) 2013 Jeff Mair. All rights reserved.
//

#ifndef __cps511a2__VertexQuad__
#define __cps511a2__VertexQuad__

#include <vector>
#include "Quad.h"
#include <iostream>

class VertexQuad {
public:
    int vertexId;
    std::vector<Quad *> quads;
};

#endif /* defined(__cps511a2__VertexQuad__) */
