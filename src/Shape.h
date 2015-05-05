//
//  Shape.h
//  cps511a1
//
//  Created by Jeff Mair on 2013-09-23.
//
//

#ifndef __cps511a1__Shape__
#define __cps511a1__Shape__

#include <iostream>
#include <vector>
#include "Vertex.h"

class RenderText;

class Shape
{
private:
    GLfloat _color[3];
    GLfloat _offset[3];
    GLfloat _scaling[3];
    std::vector<RenderText *> _renderText;
    std::vector<Shape*> _children;
protected:
    std::vector<Vertex3> _verticies;
public:
    int shapeType;
    void addVertex(GLfloat x, GLfloat y, GLfloat z);
    virtual void setOffset(GLfloat x, GLfloat y, GLfloat z);
    virtual void setColor(GLfloat r, GLfloat g, GLfloat b);
    virtual void setScaling(GLfloat x, GLfloat y, GLfloat z);
    GLfloat * getColor();
    std::vector<Vertex3> getVerticies();
    GLfloat * getOffset();
    GLfloat * getScaling();
    std::vector<Shape*> getChildren();
    void addChild(Shape* c);
    void addRenderText(RenderText*);
    std::vector<RenderText*> getRenderTexts();

};


#endif /* defined(__cps511a1__Shape__) */
