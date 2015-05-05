//
//  Ship.h
//  cps511a03
//
//  Created by Jeff Mair on 2013-11-09.
//  Copyright (c) 2013 Jeff Mair. All rights reserved.
//

#ifndef __cps511a03__Ship__
#define __cps511a03__Ship__

#include <iostream>
#include "Part.h"
#include "Projectile.h"
#include <math.h>

#define TRANSFORMATION_SCALE 2
#define TRANSFORMATION_POSIT 3

#define TRANSFORMATION_POSIT2 4
#define TRANSFORMATION_ROT_H2 5
#define SHIP_PROJECTILE_DOWN_ANGLE 120

#define ID_FLIGHTPATH_TRANSLATION 200
#define ID_FLIGHTPATH_ROTATION 201

class Ship {
private:
    Part * _body;
    Projectile * _projectile;
    
    GLfloat * getRotationHoriz();
    GLfloat * getRotationVert();
    
    float getRandFloat();
    float getRandFloat(float min, float max);
    float getRandFloat7_125();
    
    float _shipScale;
    float _heightRange;
    float _rotationSpeedAttenuation;
    float _rotationRadius;
    float _flightTerritoryZOffset;
    
    float _calculatedXPos;
    float _calculatedYPos;
    float _calculatedZPos;
    
    int _pathPosition;
    int _rollPosition;
    int _circleRotationDirection;
    
    bool _fullsizeShip;
    bool _rollLeft;
    bool _isHit;
    
    void setMaterialGrey(Part * part);
public:
    Ship(bool fullSize, GLuint textureId, Mesh * bodyMesh, Mesh * wingConnectorMesh, Mesh * wingMesh, GLfloat * textureMap);
    void move();
    void setProjectile(Projectile * p);
    void getPosition(GLfloat * x, GLfloat * y, GLfloat * z);
    Part * getBody();
    bool animateDeath();
    void setIsHit();
    bool getIsHit();
    bool getIsInactive();
};

#endif /* defined(__cps511a03__Ship__) */
