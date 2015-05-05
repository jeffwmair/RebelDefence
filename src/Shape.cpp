//
//  Shape.cpp
//  cps511a1
//
//  Created by Jeff Mair on 2013-09-23.
//
//

#include "Shape.h"
#include "RenderText.h"
using namespace std;

std::vector<Vertex3> Shape::getVerticies()
{
    return _verticies;
}
void Shape::addVertex(GLfloat x, GLfloat y, GLfloat z)
{
    Vertex3 v(x, y, z);
    _verticies.push_back(v);
}
void Shape::setColor(GLfloat r, GLfloat g, GLfloat b)
{
    _color[0] = r;
    _color[1] = g;
    _color[2] = b;
}

void Shape::setOffset(GLfloat x, GLfloat y, GLfloat z) {
    _offset[0] = x;
    _offset[1] = y;
    _offset[2] = z;
}

GLfloat * Shape::getOffset() {
    return _offset;
}

GLfloat * Shape::getColor()
{
    return _color;
}

void Shape::setScaling(GLfloat x, GLfloat y, GLfloat z) {
    _scaling[0] = x;
    _scaling[1] = y;
    _scaling[2] = z;
}

GLfloat * Shape::getScaling() { return _scaling; }

vector<Shape*> Shape::getChildren() { return _children; }
void Shape::addChild(Shape* c) { _children.push_back(c); }

void Shape::addRenderText(RenderText * r) {
    _renderText.push_back(r);
}

vector<RenderText*> Shape::getRenderTexts() {
    return _renderText;
}



