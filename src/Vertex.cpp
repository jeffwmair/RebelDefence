//
//  Vertex.cpp
//  cps511a1
//
//  Created by Jeff Mair on 2013-09-23.
//
//

#ifdef __APPLE__
#include <GLUT/glut.h>
#elif defined _WIN32 || defined _WIN64
#include <stdlib.h>
#include <glut.h>
#endif
#ifdef __linux__
#include <GL/glut.h>
#endif

#include "Vertex.h"
#include <math.h>

Vertex3::Vertex3(GLfloat inX, GLfloat inY, GLfloat inZ)
{
    x = inX;
    y = inY;
    z = inZ;
	
	_normal[0] = 0.0f;
	_normal[1] = 0.0f;
	_normal[2] = 0.0f;
}

GLfloat Vertex3::getX() { return x; }
GLfloat Vertex3::getY() { return y; }
GLfloat Vertex3::getZ() { return z; }
void Vertex3::setPosition(GLfloat xNew, GLfloat yNew, GLfloat zNew)
{
	x = xNew;
	y = yNew;
	z = zNew;
}
void Vertex3::setId(int id) { _id = id; }

void Vertex3::setNormal(GLfloat x, GLfloat y, GLfloat z) {
	_normal[0] = x;
	_normal[1] = y;
	_normal[2] = z;
}

float Vertex3::getDistance(GLfloat xOther, GLfloat yOther, GLfloat zOther) {
    float xPart = pow((x - xOther), 2);
    float yPart = pow((y - yOther), 2);
    float zPart = pow((z - zOther), 2);
    float distance = sqrt(xPart + yPart + zPart);
    return distance;
}

GLfloat * Vertex3::getNormal() {
	return _normal;
}

int Vertex3::getId() { return _id; }
