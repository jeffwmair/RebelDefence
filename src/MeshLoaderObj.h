//
//  MeshLoader.h
//  cps511a03
//
//  Created by Jeff Mair on 2013-11-30.
//  Copyright (c) 2013 Jeff Mair. All rights reserved.
//

#ifndef __cps511a03__MeshLoader__
#define __cps511a03__MeshLoader__

#include <iostream>
#include "Mesh.h"
#include <vector>
#include <fstream>
#include <sstream>

class MeshLoaderObj {
private:
    
public:
    Mesh * loadMesh(std::string filename);
};

#endif /* defined(__cps511a03__MeshLoader__) */
