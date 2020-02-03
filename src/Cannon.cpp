//
//  Cannon.cpp
//  cps511a03
//
//  Created by Jeff Mair on 2013-11-06.
//  Copyright (c) 2013 Jeff Mair. All rights reserved.
//

#include "Cannon.h"

GLfloat Cannon::getPosition(int index) {
    return _body->getTransformationById(ID_CANNON_TRANS, index);
}

void Cannon::addGrooves(Mesh * mesh) {
    for(int i = 0; i < 10; i++) {
        Part * groove = new Part(mesh, -1);
        _body->addChild(groove);
        groove->setMaterialCannonGrooves();
        groove->addRotation(90.0, 1.0, 0.0, 0.0);
        groove->addScale(0.02, 0.02, 0.05);
        groove->addTranslation(0.0, 140-i*10, 9+(i*0.4));
    }
}
bool Cannon::isRecoiling(){ return _isRecoiling; }
Cannon::Cannon(Mesh * bodyMesh, Mesh * sightMesh, bool hasGrooves) {
    
	_isRecoiling = false;
	recoilDirBack = true;
	recoilDist = 0;

    _body = new Part(bodyMesh, -1);
    _sight = new Part(sightMesh, -1);
    
    /* these are the default cannon transformations; modify if used in the walkers, etc */
    _body->addRotation(-90, 1.0, 0.0, 0.0, ID_ROTATION_Y);    // used in main to align with camera
    _body->addRotation(0.0, 1.0, 0.0, 0.0, ID_ROTATION_X);                    // used in main to align with camera
    _body->addTranslation(0, 20.0, 100.0, ID_CANNON_TRANS);
    
    if (hasGrooves) {
        addGrooves(sightMesh);   
    }

    _body->setMaterialCannon();
    recoilDirBack = true;
    
}

void Cannon::recoilBegin(GLfloat dirX, GLfloat dirY, GLfloat dirZ) {
    _isRecoiling = true;
    
    recoilDirectionAboutYTheta = 180.0 * atan(dirX / dirZ) / A_PI;
    recoilDirectionAboutXTheta = 180.0 * atan(dirY / dirZ) / A_PI;
    recoilMax = RECOIL_DISTANCE * cos(A_PI*recoilDirectionAboutXTheta/180.0);
}


void Cannon::recoilContinue() {
    
    if (!_isRecoiling) {
        return;
    }
    
    float curX = getPosition(0);
    float curY = getPosition(1);
    float curZ = getPosition(2);
    
    
    float changeAmt = RECOIL_RATE_BACK;
    if (!recoilDirBack) changeAmt = RECOIL_RATE_FWD;
    if (!recoilDirBack) {
        changeAmt *= -1;
    }
    
    float zChange = changeAmt*cos(A_PI * recoilDirectionAboutYTheta / 180.0);
    float xChange = changeAmt*sin(A_PI * recoilDirectionAboutYTheta / 180.0);
    
    recoilDist += changeAmt;
    _body->setTransformationValueWithId(ID_CANNON_TRANS, curX + xChange, curY, curZ + zChange, 0.0);
        
    if (recoilDirBack && recoilDist >= recoilMax) {
        recoilDirBack = false;
    }
    else if (!recoilDirBack && recoilDist <= 0) {
        recoilDirBack = true;
        _isRecoiling = false;
        recoilDist = 0;
    }
}

Part * Cannon::getBody() {
    return _body;
}

