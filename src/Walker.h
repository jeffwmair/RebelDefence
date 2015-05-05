//
//  Walker.h
//  cps511a03
//
//  Created by Jeff Mair on 2013-11-13.
//  Copyright (c) 2013 Jeff Mair. All rights reserved.
//

#ifndef __cps511a03__Walker__
#define __cps511a03__Walker__

#include <iostream>
#include "Part.h"
#include "Cannon.h"
#include "Projectile.h"

#define WALKER_BODY_HEIGHT 700
#define WALKER_DEATH_DURATION 60
#define WALKER_SPEED 50


#define ID_BODY_TRANSLATION 1
#define ID_NECK_ROTATION 2
#define ID_LEFTLEG_JOINT_TOP_ROT 3
#define ID_LEFTLEG_JOINT_MID_ROT 4
#define ID_RIGHTLEG_JOINT_TOP_ROT 5
#define ID_RIGHTLEG_JOINT_MID_ROT 6

#define ID_CANNON_JOINT 1000

class Walker {
private:
    Part * _body;
    Projectile * _projectile;
    Cannon * _cannon;
    
    bool rotateHeadLeft;
    bool bodyBobbingDown;
    bool jointARotationUp;
    bool jointBRotationUp;
    bool _isActive;
    bool _isDying;
    
    int _hitCount;
    float _speedRandomFactor;
    float _angleDegsWrtCannonX;
    float _angleDegsWrtCannonY;
    
    GLfloat _startPos[3];
    void changeMaterialDuringDeath(Part * p);
    void setMaterial(Part * p);
    void makeInactive();
    Part * getHead();
    
public:
    Walker(GLuint headTextureId, GLuint bodyTextureId,
           Mesh * headMesh, Mesh * cylindarMesh, Mesh * cubeMesh,
           GLfloat * cubeMap, GLfloat * cylinderTextureMap, Cannon * cannon);
    void setProjectile(Projectile * p);

    int _animationsAfterBeingHit;
    Part * getBody();

    void walk(float speed);
    GLfloat * getPosition();
    GLfloat * getPositionOfHead();
    void start();
    void resetToStartPosition();
    void setStartPosition(GLfloat x, GLfloat y, GLfloat z);
    bool animateDeath();
    void setIsHit();
    bool getIsDying();
    bool getIsInactive();
};


#endif /* defined(__cps511a03__Walker__) */
