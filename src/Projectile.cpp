//
//  Projectile.cpp
//  cps511a03
//
//  Created by Jeff Mair on 2013-11-09.
//  Copyright (c) 2013 Jeff Mair. All rights reserved.
//

#include "Projectile.h"
#include "Sound.h"
#define PI 3.14159

using namespace std;

Projectile::Projectile(bool fast, Mesh * bodyMesh, float scale) {
    
    _speed = (fast) ? PROJECTILE_SPEED_FAST : PROJECTILE_SPEED;
    _body = new Part(bodyMesh, -1);
    _isFiring = false;
    _distanceFired = 0;
    
    _body->addScale(scale, 2.5*scale, scale);
    _body->addRotation(0.0, 1.0, 0.0, 0, ID_ROTATION_Y);
    _body->addRotation(0.0, 0.0, 1.0, 0, ID_ROTATION_X);
    _body->addTranslation(0, 0, 0, ID_TRANSLATION);
       
    _body->isVisible = false;
}

Part * Projectile::getPart() { return _body; }

bool Projectile::isColliding(GLfloat partPosition0, GLfloat partPosition1, GLfloat partPosition2) {
    
    GLfloat x = partPosition0;
    GLfloat y = partPosition1;
    GLfloat z = partPosition2;
	GLfloat px = getPosition(0);
	GLfloat py = getPosition(1);
	GLfloat pz = getPosition(2);
    
#define COLLISION_RANGE 150
    
    // need some leaway in the Z range since the projectile moves pretty fast
    // also, the "body" of the walker is really the shoulders part, so give extra room tehre
    bool withinX = (fabs(x - px) < COLLISION_RANGE);
    bool withinY = (fabs(y - py) < COLLISION_RANGE);
    bool withinZ = (fabs(z - pz) < COLLISION_RANGE*15);
    
    return (withinX && withinY && withinZ);
}

void Projectile::fireBegin(GLfloat angleHoriz, GLfloat angleVert, float startX, float startY, float startZ) {

    _body->isVisible = true;
    setIsFiring(true);
    
    _body->setTransformationValueWithId(ID_ROTATION_Y, -angleVert, 1.0, 0, 0);
    _body->setTransformationValueWithId(ID_ROTATION_X, 180+angleHoriz, 0.0, 1.0, 0.0);
    _body->setTransformationValueWithId(ID_TRANSLATION, startX, startY, startZ, 0);
    
    _angleAtTimeOfFiring[0] = _body->getTransformationById(ID_ROTATION_X, 0);
	_angleAtTimeOfFiring[1] = _body->getTransformationById(ID_ROTATION_Y, 0);
    // first rotation is about Y; first element is angle in degrees
    // second rotation is about X; first element is angle in degrees
    
    // save the home position
	_homePosition[0] = getPosition(0);
	_homePosition[1] = getPosition(1);
	_homePosition[2] = getPosition(2);
}


bool Projectile::fireContinued() {
    
    if (!_isFiring) {
        return false;
    }
       
    _distanceFired += _speed;
    
	GLfloat oldX = getPosition(0);
	GLfloat oldY = getPosition(1);
	GLfloat oldZ = getPosition(2);
    
    float angleAboutYRad = PI * (_angleAtTimeOfFiring[0]) / 180.0;
    float angleAboutXRad = PI * (_angleAtTimeOfFiring[1]) / 180.0;
    
    float zDist = _speed * sin(angleAboutXRad) * cos(angleAboutYRad);
    float xDist = _speed * sin(angleAboutXRad) * sin(angleAboutYRad);
    float yDist = _speed * cos(angleAboutXRad);
    
    _body->setTransformationValueWithId(ID_TRANSLATION, oldX + xDist, oldY + yDist, oldZ + zDist, 0);
    
    if (_distanceFired >= PROJECTILE_RANGE) {
        setIsFiring(false);
        return false;
    }
    
    return true;
}

void Projectile::finishFiring() {
    _body->isVisible = false;
    // reset to home position
    _distanceFired = 0;
    _body->setTransformationValueWithId(ID_TRANSLATION, _homePosition[0], _homePosition[1], _homePosition[2], 0);
}

GLfloat Projectile::getPosition(int index) {
	return _body->getTransformationById(ID_TRANSLATION, index);
}

void Projectile::setIsFiring(bool isFiring) {
    
    if (_isFiring && !isFiring) {
        finishFiring();
    }
    _isFiring = isFiring;
}

bool Projectile::isFiring() { return _isFiring; }