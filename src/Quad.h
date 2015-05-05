/*
 *  Quad.h
 *  cps511a2
 *
 *  Created by Jeff on 13-11-01.
 *  Copyright 2013 Jeff Mair. All rights reserved.
 *
 */

#include "Vertex.h"

class Quad {
private:
	GLfloat _normal[3];
public:
//  	GLfloat verticies[12];
//    GLfloat vertexNormals[12];
  	Vertex3 * verticies[4];
	Quad(Vertex3 * v1, Vertex3 * v2, Vertex3 * v3, Vertex3 * v4);
	void setNormal(GLfloat x, GLfloat y, GLfloat z);
//    Vertex3 * getVerticies();
	GLfloat * getNormal();
};