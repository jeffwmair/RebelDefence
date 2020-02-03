//
//  Cannon.h
//  cps511a03
//
//  Created by Jeff Mair on 2013-11-06.
//  Copyright (c) 2013 Jeff Mair. All rights reserved.
//

#ifndef __cps511a03__Cannon__
#define __cps511a03__Cannon__

#define RECOIL_RATE_BACK 4
#define RECOIL_RATE_FWD 0.75
#define RECOIL_DISTANCE 10
#define A_PI 3.1415926

#define ID_CANNON_TRANS 1

#include <iostream>
#include <math.h>

#include "Mesh.h"
#include "Part.h"

class Cannon {
private:
    Part * _body;
    Part * _sight;
    void addGrooves(Mesh * mesh);
    bool _isRecoiling;
    bool recoilDirBack;
    float recoilDist;
    float recoilDirectionAboutYTheta, recoilDirectionAboutXTheta, recoilMax;
public:
    void recoilBegin(GLfloat dirX, GLfloat dirY, GLfloat dirZ);
    void recoilContinue();
    bool isRecoiling();
    Cannon(Mesh * bodyMesh, Mesh * sightMesh, bool hasGrooves);
    Part * getBody();
    GLfloat getPosition(int index);

};

#endif /* defined(__cps511a03__Cannon__) */
