/*
 *  Quad.cpp
 *  cps511a2
 *
 *  Created by Jeff on 13-11-01.
 *  Copyright 2013 Jeff Mair. All rights reserved.
 *
 */

#include "Quad.h"
#include "MyMath.h"

void getVertexCrossProduct(Vertex3 *v1, Vertex3 *v2, Vertex3 *v3, GLfloat normal[3]);

Quad::Quad(Vertex3 * v1, Vertex3 * v2, Vertex3 * v3, Vertex3 * v4) {
	verticies[0] = v1;
	verticies[1] = v2;
	verticies[2] = v3;
	verticies[3] = v4;
    
//    int ivertex = 0;
//    verticies[ivertex++] = v1->getX();
//    verticies[ivertex++] = v1->getY();
//    verticies[ivertex++] = v1->getZ();
//    verticies[ivertex++] = v2->getX();
//    verticies[ivertex++] = v2->getY();
//    verticies[ivertex++] = v2->getZ();
//    verticies[ivertex++] = v3->getX();
//    verticies[ivertex++] = v3->getY();
//    verticies[ivertex++] = v3->getZ();
//    verticies[ivertex++] = v4->getX();
//    verticies[ivertex++] = v4->getY();
//    verticies[ivertex++] = v4->getZ();
	
    // calc the normal vector
    GLfloat normal[3];
    getVertexCrossProduct(v1, v2, v3, normal);
	_normal[0] = normal[0];
	_normal[1] = normal[1];
	_normal[2] = normal[2];
}

GLfloat * Quad::getNormal() {
    return _normal;
}

void getVertexCrossProduct(Vertex3 *v1, Vertex3 *v2, Vertex3 *v3, GLfloat normal[3]) {

    GLfloat a1 = v1->getX() - v2->getX();
    GLfloat a2 = v1->getY() - v2->getY();
    GLfloat a3 = v1->getZ() - v2->getZ();
    
    GLfloat b1 = v3->getX() - v2->getX();
    GLfloat b2 = v3->getY() - v2->getY();
    GLfloat b3 = v3->getZ() - v2->getZ();
    
    getCrossProduct(a1, a2, a3, b1, b2, b3, normal);
  
}

