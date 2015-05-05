//
//  World.h
//  cps511a1
//
//  Created by Jeff Mair on 2013-09-23.
//
//

#ifndef __cps511a1__World__
#define __cps511a1__World__


#include <iostream>
#include <vector>
#include "VertexQuad.h"
#include "Slice.h"

#define PI 3.141592653589793
#define STATUS_BAR_BOX_HEIGHT 6
#define STATUS_BAR_TEXT_SCALE 0.05
#define NUM_VERTICIES_PER_QUAD 4
#define NUM_POINTS_PER_VERTEX 3


class Mesh {
private:
	int _numVertices;

public:
    
    /* made these public temporarily while refactoring */
    GLfloat * _verticiesAll;
    GLfloat * _normalsAll;
    	
	GLfloat * getVerticies();
	GLfloat * getNormals();
    int getNumVerticiesTotal();
    void setTotalNumVerticies(int num);
    
};


#endif /* defined(__cps511a1__World__) */
