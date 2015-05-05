//
//  PartTransformation.h
//  cps511a03
//
//  Created by Jeff Mair on 2013-11-13.
//  Copyright (c) 2013 Jeff Mair. All rights reserved.
//

#ifndef __cps511a03__PartTransformation__
#define __cps511a03__PartTransformation__

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/OpenGL.h>
#elif defined _WIN32 || defined _WIN64
#include <string>
#include <stdlib.h>
#include <glut.h>
#endif

#include <iostream>


#define OP_ROTATE 1
#define OP_SCALE 2
#define OP_TRANSLATE 3

class PartTransformation {
private:
    int _opType;
    int _id;
    GLfloat _values[4];
public:
    PartTransformation(int type, GLfloat val0, GLfloat val1, GLfloat val2, GLfloat val3, int id);
    int getOpType();
    int getId();
    GLfloat * getValues();
    void setValues(GLfloat val0, GLfloat val1, GLfloat val2, GLfloat val3);

};

#endif /* defined(__cps511a03__PartTransformation__) */
