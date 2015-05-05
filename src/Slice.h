/*
 *  Slice.h
 *  cps511a2
 *
 *  Created by Jeff on 13-11-01.
 *  Copyright 2013 Jeff Mair. All rights reserved.
 *
 */

#ifndef __cps511a1__SLICE__
#define __cps511a1__SLICE__

#include "Vertex.h"
#include <iostream>
#include <vector>

class Slice {
public:
	Slice();
    bool hasPrevious;
    std::vector<Vertex3 *> verticies;
	Slice * previous;
};

#endif