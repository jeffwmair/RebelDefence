//
//  Rectangle.h
//  cps511a1
//
//  Created by Jeff Mair on 2013-10-02.
//
//

#ifndef __cps511a1__Rectangle__
#define __cps511a1__Rectangle__

#include <iostream>
#include "Shape.h"

class RectangleJM : public Shape {
private:
public:
    RectangleJM(int shapeType);
    bool containsPoint(GLfloat x, GLfloat y);
};

#endif /* defined(__cps511a1__Rectangle__) */
