//
//  Vertex.h
//  cps511a1
//
//  Created by Jeff Mair on 2013-09-23.
//
//

#ifndef __cps511a1__Vertex__
#define __cps511a1__Vertex__

#ifdef __APPLE__
#include <GLUT/glut.h>
#elif defined _WIN32 || defined _WIN64
#include <stdlib.h>
#include <glut.h>
#endif

#include <iostream>


class Vertex3
{
private:
    int _id;
    GLfloat x, y, z;
	GLfloat _normal[3];
public:
    Vertex3(GLfloat x, GLfloat y, GLfloat z);
	void setNormal(GLfloat x, GLfloat y, GLfloat z);
    void setId(int id);
    int getId();
	GLfloat * getNormal();
    GLfloat getX();
    GLfloat getY();
    GLfloat getZ();
	void setPosition(GLfloat x, GLfloat y, GLfloat z);
    float getDistance(GLfloat x, GLfloat y, GLfloat z);
};

#endif /* defined(__cps511a1__Vertex__) */
