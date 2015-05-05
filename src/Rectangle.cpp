//
//  Rect.cpp
//  cps511a1
//
//  Created by Jeff Mair on 2013-10-02.
//
//

#include "Rectangle.h"

RectangleJM::RectangleJM(int pShapeType) {
    /* can use gl transformations to adjust it as needed */
    setOffset(0, 0, 0);
    addVertex(0, 0, 0);
    addVertex(1, 0, 0);
    addVertex(1, -1, 0);
    addVertex(0, -1, 0);
    shapeType = pShapeType;
}

bool RectangleJM::containsPoint(GLfloat x, GLfloat y) {

    GLfloat leftX = getOffset()[0];
    GLfloat rightX = getOffset()[0] + getScaling()[0];
    GLfloat topY = getOffset()[1];
    GLfloat bottomY = getOffset()[1] - getScaling()[1];
    
    bool withinX = (x >= leftX && x <= rightX);
    bool withinY = (y >= bottomY && y <= topY);
    
    return withinX && withinY;
}