
//  Projectile.h
//  cps511a03
//
//  Created by Jeff Mair on 2013-11-09.
//  Copyright (c) 2013 Jeff Mair. All rights reserved.
//

#ifndef __cps511a03__Projectile__
#define __cps511a03__Projectile__

#include <iostream>
#include <math.h>
#include "Mesh.h"
#include "Part.h"

#define PROJECTILE_SPEED 140
#define PROJECTILE_SPEED_FAST 200
#define PROJECTILE_RANGE 20000

class Projectile {
private:
    
    Part * _body;
    bool _isFiring;
    float _angleAtTimeOfFiring[2];
    float _distanceFired;
    GLfloat _homePosition[3];
    void finishFiring();
    float _speed;

public:
    
    Projectile(bool fast, Mesh * mesh, float scale);
    Part * getPart();
    void fireBegin(GLfloat angleHoriz, GLfloat angleVert, float startX, float startY, float startZ);
    void setIsFiring(bool isFiring);
    
    bool isColliding(GLfloat * partPosition);
    bool fireContinued();
    bool isFiring();

    GLfloat * getPosition();
    
};

#endif /* defined(__cps511a03__Projectile__) */
