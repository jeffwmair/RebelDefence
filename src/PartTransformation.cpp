//
//  PartTransformation.cpp
//  cps511a03
//
//  Created by Jeff Mair on 2013-11-13.
//  Copyright (c) 2013 Jeff Mair. All rights reserved.
//

#include "PartTransformation.h"

PartTransformation::PartTransformation(int type, GLfloat val0, GLfloat val1, GLfloat val2, GLfloat val3, int id) {
    _id = id;
    _opType = type;
    _values[0] = val0;
    _values[1] = val1;
    _values[2] = val2;
    _values[3] = val3;

}

int PartTransformation::getOpType() { return _opType; }
int PartTransformation::getId() { return _id; }
GLfloat * PartTransformation::getValues() { return _values; }

void PartTransformation::setValues(GLfloat val0, GLfloat val1, GLfloat val2, GLfloat val3) {
    _values[0] = val0;
    _values[1] = val1;
    _values[2] = val2;
    _values[3] = val3;
}