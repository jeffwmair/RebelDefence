//
//  Ship.cpp
//  cps511a03
//
//  Created by Jeff Mair on 2013-11-09.
//  Copyright (c) 2013 Jeff Mair. All rights reserved.
//

#include "Ship.h"
#include "Sound.h"


using namespace std;



#define SHIP_MOVEMENT_DISTANCE 45
#define SHIP_WOBBLE_X_AMT_MAX 50
#define SHIP_WOBBLE_X_AMT 5
#define SHIP_DESCENT_RATE 1
#define SHIP_SPIN pow(_spinCount,1.3)

#define SHIP_SCALE_FULL 1.5
#define SHIP_SCALE_SM 0.9

#define WING_CONNECTOR_LENGTH 1.5
#define WING_CONNECTOR_OFFSETS 85

#define WING_HEIGHT 6
#define WING_LENGTH 3
#define WING_WIDTH 0.025

#define WING_H_POS 130
#define WING_V_POS -385

#define SHIP_HEIGHT 200

/*  todo: refactor this into base class of walker and ship */
float Ship::getRandFloat() {
    // random number between 0.5 and 2
    return 0.5 + ((rand() % 1500) / 1000.0f);
}

float Ship::getRandFloat(float min, float max) {
    // random number between 0.5 and 2
    return min + ((rand() % (int)((max-min)*1000.0)) / 1000.0f);
}


float Ship::getRandFloat7_125() {
    // random number between 0.7 and 1
    return 0.7 + ((rand() % 550) / 1000.0f);
}

void Ship::setProjectile(Projectile *p) {
    _projectile = p;
}

void Ship::getPosition(GLfloat * x, GLfloat * y, GLfloat * z) {
    *x = _calculatedXPos;
    *y = _calculatedYPos;
    *z = _calculatedZPos;
}

GLfloat Ship::getRotationHoriz(int index) {
    return _body->getTransformationById(ID_ROTATION_X, index);

}

GLfloat Ship::getRotationVert(int index) {
	return _body->getTransformationById(ID_ROTATION_Y, index);
}

void Ship::move() {
    
    _pathPosition++;
    
    if (_isHit) {
        _rollPosition = (_rollPosition + 1) % 360;
    }
    else {
        if (_rollLeft) {
            _rollPosition++;
            if (_rollPosition == 180 && !_isHit) _rollLeft = false;
        }
        else {
            _rollPosition--;
            if (_rollPosition == 0) _rollLeft = true;
        }
    }
    
    float rollPositionRad = 3.14159 * _rollPosition / 180.0;
    float rollAmount = 0;
    if (_isHit) {
        rollAmount = -1;
    }
    else {
        rollAmount = cos(rollPositionRad);
    }
       
    // rotate in a big circle
    int flightCircleCurrentAngle = 0;
    
    // roll from left to right
	//PartTransformation *pt = _body->getTransformationById(ID_ROTATION_Y);
	//cout << pt->getValues()[0] << endl;
	float currentRoll = _body->getTransformationById(ID_ROTATION_Y, 0);
    float newRoll = currentRoll + ((_isHit) ? rollAmount*3 : rollAmount/4);
    _body->setTransformationValueWithId(ID_ROTATION_Y, newRoll, 1, 0, 0);
    
    // bob up and down
    float currentX = _body->getTransformationById(ID_FLIGHTPATH_TRANSLATION, 0);  // note that this X doesnt actually reflect position!
    float currentY = _body->getTransformationById(ID_FLIGHTPATH_TRANSLATION, 1);
    float currentZ = _body->getTransformationById(ID_FLIGHTPATH_TRANSLATION, 2);  // note that this Z doesnt actually reflect position!
    float newHeight = currentY + ((_isHit) ? (rollAmount * 15) : (rollAmount*5));
    _body->setTransformationValueWithId(ID_FLIGHTPATH_TRANSLATION, currentX, newHeight, currentZ, 0);
    
    flightCircleCurrentAngle = _circleRotationDirection * (int)(_pathPosition / _rotationSpeedAttenuation) % 360;
    _body->setTransformationValueWithId(ID_FLIGHTPATH_ROTATION, flightCircleCurrentAngle, 0, 1, 0);
    
    /* as long as the ship is rotating, its alive, so update its position */
    _calculatedXPos = _rotationRadius * sinf(3.14159 * flightCircleCurrentAngle / 180.0);
    _calculatedYPos = newHeight;
    _calculatedZPos = _flightTerritoryZOffset + _rotationRadius * cosf(3.14159 * flightCircleCurrentAngle / 180.0);
    
    if (!_isHit && getRandFloat(0, 10.0) < 0.04) {
        Sound::playCannonFireQuiet();
        _projectile->fireBegin(flightCircleCurrentAngle, 120, _calculatedXPos, _calculatedYPos, _calculatedZPos);
    }
 
}

bool Ship::animateDeath() {
    move();
    return false;
}

void Ship::setIsHit() {
    _isHit = true;
}

bool Ship::getIsHit() {
    return _isHit;
}

#define FLIGHT_TERRITORY_Z -12000
Ship::Ship(bool fullSizeShip, GLuint textureId, Mesh * bodyMesh, Mesh * wingConnectorMesh, Mesh * wingMesh, GLfloat * textureMap) {
    
    _isHit = false;
    _rollPosition = 1;
    _rotationSpeedAttenuation = getRandFloat(2, 4);
    _circleRotationDirection = (getRandFloat() < 1.2) ? 1 : -1;
    _rotationRadius = -5000 * getRandFloat();
    _flightTerritoryZOffset = FLIGHT_TERRITORY_Z * getRandFloat();
    _rollLeft = true;
    _pathPosition = 0;
    _shipScale = (fullSizeShip) ? SHIP_SCALE_FULL : SHIP_SCALE_SM;
    _calculatedXPos = 0.0f;
    _calculatedYPos = 0.0f;
    _calculatedZPos = 0.0f;
    
    float shipFacingDirectionAngle = (_circleRotationDirection == 1) ? 0.0f : 180.0f;
    
    _body = new Part(textureId, textureMap, bodyMesh, -1);
    _body->addRotation(shipFacingDirectionAngle, 0.0, 1.0, 0.0, ID_ROTATION_X);
    _body->addRotation(0.0, 1.0, 0.0, 0.0, ID_ROTATION_Y);
    _body->addScale(_shipScale, _shipScale*1.5, _shipScale);
    _body->addTranslation(0, 0, 0, ID_TRANSLATION);
    
    _body->addTranslation(0, 0, _rotationRadius); // rotation radius
    _body->addRotation(0, 0, 1, 0, ID_FLIGHTPATH_ROTATION);    // placeholder for rotating
    _body->addTranslation(0, 2500*getRandFloat(0.5,2.5), _flightTerritoryZOffset, ID_FLIGHTPATH_TRANSLATION); // move the centre of rotation
    
    Part * wingConnector1 = new Part(wingConnectorMesh, -1);
    Part * wing1 = new Part(wingMesh, -1);

    Part * wingConnector2 = new Part(wingConnectorMesh, -1);
    Part * wing2 = new Part(wingMesh, -1);

    wingConnector1->addRotation(90, 1.0, 0.0, 0.0);
    wingConnector1->addScale(1, 0.5, WING_CONNECTOR_LENGTH);
    wingConnector1->addTranslation(0, WING_CONNECTOR_OFFSETS, WING_CONNECTOR_OFFSETS);
    
    
    wingConnector2->addRotation(-90, 1.0, 0.0, 0.0);
    wingConnector2->addScale(1, 0.5, WING_CONNECTOR_LENGTH);
    wingConnector2->addTranslation(0, WING_CONNECTOR_OFFSETS, -WING_CONNECTOR_OFFSETS);

    wing1->addRotation(90.0, 1.0, 0.0, 0.0);
    wing1->addScale(WING_LENGTH, WING_WIDTH, WING_HEIGHT);
    wing1->addTranslation(0, WING_H_POS, WING_V_POS);
    
    wing2->addRotation(90.0, 1.0, 0.0, 0.0);
    wing2->addScale(WING_LENGTH, WING_WIDTH, WING_HEIGHT);
    wing2->addTranslation(0, WING_H_POS, WING_V_POS);
    
    _body->addChild(wingConnector1);
    _body->addChild(wingConnector2);
    wingConnector1->addChild(wing1);
    wingConnector2->addChild(wing2);

    setMaterialGrey(_body);
    setMaterialGrey(wingConnector1);
    setMaterialGrey(wingConnector2);
    setMaterialGrey(wing1);
    setMaterialGrey(wing2);
    
}


void Ship::setMaterialGrey(Part * part) {
    
    part->setMaterialAmbient(0.4, 0.4, 0.4, 1.0);
    part->setMaterialSpecular(0.1,0.1,0.1, 1.0);
    part->setMaterialDiffuse(0.35, 0.35, 0.4, 1.0);
    part->setMaterialShininess(0.2);
}


Part * Ship::getBody() { return _body; }





