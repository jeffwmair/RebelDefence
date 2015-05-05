//
//  Walker.cpp
//  cps511a03
//
//  Created by Jeff Mair on 2013-11-13.
//  Copyright (c) 2013 Jeff Mair. All rights reserved.
//

#include "Walker.h"
#include <math.h>
#include "Sound.h"

#define WALKER_SCALE 1
#define OVERALL_ROTATION -45.0
#define BODY_SCALE 2.5


#define BACK_JOINT_COLLAPSE_RATE 0.75f
#define DEATH_FALL_INVERSE_FACTOR 5

using namespace std;

float getRandFloat() {
    // random number between 0.5 and 2
    return 0.5 + ((rand() % 1500) / 1000.0f);
}

/*
 Reminder for later:  we have to do this pre and post rotate business
 due to the order of matrix operations in opengl - the first operations applied
 in the code (which would be the parent operations the way I have it setup)
 are the last ones applied in the pipeline.  A little confusing, but that's the issue.
 */
void preRotate(Part * p) {
    p->addRotation(OVERALL_ROTATION, 0.0, 1.0, 0.0);
}

void postRotate(Part * p) {
    p->addRotation(-OVERALL_ROTATION, 0.0, 1.0, 0.0);
}

void Walker::makeInactive() {
    _isActive = false;
    _isDying = false;
    _hitCount = 0;
}

void Walker::start() {
    _speedRandomFactor = WALKER_SPEED * getRandFloat();
    _isActive = true;
    setMaterial(_body);
}

void Walker::setStartPosition(GLfloat x, GLfloat y, GLfloat z) {
    _startPos[0] = x;
    _startPos[1] = y;
    _startPos[2] = z;
}

void Walker::resetToStartPosition() {
    _animationsAfterBeingHit = 0;
    makeInactive();
    _body->setTransformationValueWithId(ID_BODY_TRANSLATION, _startPos[0], _startPos[1], _startPos[2], 0);
    
    /* reset the joints because the legs will be awkwardly positioned after being destroyed */
    Part * leftJoint = _body->getChildren()[1];
    Part * rightJoint = _body->getChildren()[2];
    Part * leftJointB = leftJoint->getChildren()[0]->getChildren()[0];
    Part * rightJointB = rightJoint->getChildren()[0]->getChildren()[0];
    leftJoint->setTransformationValueWithId(ID_LEFTLEG_JOINT_TOP_ROT, 0, 1.0, 0.0, 0.0);
    rightJoint->setTransformationValueWithId(ID_RIGHTLEG_JOINT_TOP_ROT, 0, 1.0, 0.0, 0.0);
    leftJointB->setTransformationValueWithId(ID_LEFTLEG_JOINT_MID_ROT, 0, 0, 1.0, 0);
    rightJointB->setTransformationValueWithId(ID_RIGHTLEG_JOINT_MID_ROT,0, 0, 1.0, 0);

    setMaterial(_body);
}

GLfloat * Walker::getPosition() {
    return _body->getTransformationById(ID_BODY_TRANSLATION)->getValues();
}

GLfloat * Walker::getPositionOfHead() {
    GLfloat * posBody = getPosition();
    posBody[1] = posBody[1] + 250;
    return posBody;
}

void Walker::changeMaterialDuringDeath(Part * p) {
    for(int i = 0; i < p->getChildCount(); i++) {
        changeMaterialDuringDeath(p->getChildren()[i]);
    }
    
    GLfloat * matAmbient = p->getMaterialAmbient();
    GLfloat * matDiffuse = p->getMaterialDiffuse();
    matAmbient[0] += 0.005;
    matDiffuse[0] += 0.01;

}

void Walker::setMaterial(Part * p) {
    for(int i = 0; i < p->getChildCount(); i++) {
        setMaterial(p->getChildren()[i]);
    }
    p->setMaterialMetal();
}


bool Walker::animateDeath() {
    
    _animationsAfterBeingHit++;
    getHead()->setMaterialMetal();
    
    // change colour
//    changeMaterialDuringDeath(_body);

    // fall downward
    GLfloat curX = getPosition()[0];
    GLfloat curY = getPosition()[1];
    GLfloat curZ = getPosition()[2];
    
    if (_animationsAfterBeingHit <= WALKER_DEATH_DURATION) {

        _body->setTransformationValueWithId(ID_BODY_TRANSLATION, curX, curY - (_animationsAfterBeingHit/DEATH_FALL_INVERSE_FACTOR), curZ, 0);
        
        // have him collapse somewhat
        // ROTATE THE FIRST JOINTS
        Part * leftJoint = _body->getChildren()[1];
        Part * rightJoint = _body->getChildren()[2];
        Part * leftJointB = leftJoint->getChildren()[0]->getChildren()[0];
        Part * rightJointB = rightJoint->getChildren()[0]->getChildren()[0];
        
        GLfloat curLegRotation;
        if (_animationsAfterBeingHit == 1) {
            curLegRotation = 0;
        }
        else {
            curLegRotation = leftJoint->getTransformationById(ID_LEFTLEG_JOINT_TOP_ROT)->getValues()[0];
        }
    
        GLfloat newRotation = curLegRotation + (4.5f/DEATH_FALL_INVERSE_FACTOR);
        

        leftJoint->setTransformationValueWithId(ID_LEFTLEG_JOINT_TOP_ROT, newRotation, 1.0, 0.0, 0.0);
        rightJoint->setTransformationValueWithId(ID_RIGHTLEG_JOINT_TOP_ROT, newRotation, 1.0, 0.0, 0.0);
        leftJointB->setTransformationValueWithId(ID_LEFTLEG_JOINT_MID_ROT, newRotation*BACK_JOINT_COLLAPSE_RATE, 0, 1.0, 0);
        rightJointB->setTransformationValueWithId(ID_RIGHTLEG_JOINT_MID_ROT, newRotation*BACK_JOINT_COLLAPSE_RATE, 0, 1.0, 0);
        
        return true;
    }
    else {
        //resetToStartPosition();
        _isDying = false;
        _isActive = false;
        return false;
    }
}


void Walker::walk(float unused) {
    
    getHead()->setMaterialMetal();
    
    if (!_isActive) return;
    
    _cannon->recoilContinue();
   
    /*
     
     ANIMATE THE HEAD 
     
     */
    float maxHeadHDegrees = 70;
    float headRotationRate = _speedRandomFactor * 0.015;
    
    /* rotate the neck */
    Part * neck = _body->getChildren()[0];
    GLfloat curRotation = neck->getTransformationById(ID_NECK_ROTATION)->getValues()[0];
        
    float curWalkerX = getPosition()[0];
    float curWalkerZ = getPosition()[2];

    
    if (_hitCount > 0) {
        
        /* train his gaze on the user */
        if (fabsf(curRotation - _angleDegsWrtCannonX) < 1) {
            headRotationRate = 0;
        }
        else {
            headRotationRate = (curRotation > _angleDegsWrtCannonX) ? -1 : 1;
        }
        
        // keep the cannon trained on the user too
        getBody()->getChildById(ID_CANNON_JOINT)->setTransformationValueWithId(ID_ROTATION_Y, -_angleDegsWrtCannonY, 1, 0, 0);

        // fire more frequently, and directly at the user
        if (getRandFloat() < 0.525) {
            _projectile->fireBegin(curRotation, 90-(_angleDegsWrtCannonY), curWalkerX, 850, curWalkerZ);
            Sound::playCannonFireQuiet();
        }
    }
    else {
        float randomFireChance = getRandFloat();
        if (curRotation > maxHeadHDegrees) {
            rotateHeadLeft = true;
        }
        else if (curRotation < -maxHeadHDegrees) {
            rotateHeadLeft = false;
        }
        else if (randomFireChance < 0.501) {
            
            rotateHeadLeft = !rotateHeadLeft; // look the other direction after firing
            
            if (!_cannon->isRecoiling()) {
                
                // unfortunately since the projectile isnt' really attached to the cannon
                // we can't take advantage of the canon position to correctly position the
                // projectile.  So hardcoding the height and vertical angle to roughly
                // match the cannon position.
//                _cannon->recoilBegin(curRotation, 90.0, 0);
                
                _projectile->fireBegin(curRotation, 105.0, curWalkerX, 850, curWalkerZ);
                Sound::playCannonFireQuiet();
            }
        }
        if (rotateHeadLeft) {
            headRotationRate *= -1;
        }
    }

    
    headRotationRate *= getRandFloat();
    curRotation += headRotationRate * cosf(3.14 * curRotation / 180.0);
    neck->setTransformationValueWithId(ID_NECK_ROTATION, curRotation, 0, 1.0, 0);
    
    /* WHILE WALKING, THE BODY GOES UP AND DOWN */
    
    
    /* speeds of 16+ look okay */
    float forwardSpeed = 0.1 * _speedRandomFactor;
    float maxBobRange = 40.0;
    float bobRateAmount = forwardSpeed * 0.75;

    
    float maxJointARotation = 25.0;
    float jointARotationAmt = bobRateAmount * 0.29;
    float maxJointBRotation = maxJointARotation / 1.5;
    float jointBRotationAmt = jointARotationAmt / 1.5;
   
    
    GLfloat curX = getPosition()[0];
    GLfloat curY = getPosition()[1];
    GLfloat curZ = getPosition()[2];
    
    float curBobLevel = curY - WALKER_BODY_HEIGHT;
    
    if (curBobLevel > maxBobRange) {
        bodyBobbingDown = true;
    }
    else if (curBobLevel < -maxBobRange) {
        bodyBobbingDown = false;
    }
    
    if (bodyBobbingDown) {
        bobRateAmount *= -1.0;
    }
    
    GLfloat newHeight = curY + bobRateAmount;
    GLfloat newZ = curZ + forwardSpeed;

    // ALSO WALK FORWARD AT THE SAME TIME
    _body->setTransformationValueWithId(ID_BODY_TRANSLATION, curX, newHeight, newZ, 0);
    
    /* keep track of the body wrt the user's cannon so the walker can look at the cannon when hit */
    _angleDegsWrtCannonX = (180.0/3.14159)*atan(curX / newZ);
    _angleDegsWrtCannonY = (180.0/3.14159)*atan(WALKER_BODY_HEIGHT / newZ);
    
    // ROTATE THE FIRST JOINTS
    Part * leftJointTop = _body->getChildren()[1];
    Part * rightJointTop = _body->getChildren()[2];
    
    
    GLfloat curRotationLeftA = leftJointTop->getTransformationById(ID_LEFTLEG_JOINT_TOP_ROT)->getValues()[0];
    if (curRotationLeftA >= maxJointARotation) {
        jointARotationUp = false;
    }
    else if (curRotationLeftA <= -maxJointARotation) {
        jointARotationUp = true;
    }
    
    if (!jointARotationUp) {
        jointARotationAmt *= -1;
    }
    
    GLfloat newRotationJointALeft = curRotationLeftA + jointARotationAmt;
    
    leftJointTop->setTransformationValueWithId(ID_LEFTLEG_JOINT_TOP_ROT, newRotationJointALeft, 1.0, 0.0, 0.0);
    rightJointTop->setTransformationValueWithId(ID_RIGHTLEG_JOINT_TOP_ROT, -newRotationJointALeft, 1.0, 0.0, 0.0);
    
    
    // ROTATE THE SECOND JOINT
    Part * leftJointB = leftJointTop->getChildren()[0]->getChildren()[0];
    Part * rightJointB = rightJointTop->getChildren()[0]->getChildren()[0];
    
    GLfloat curRotationJointB = leftJointB->getTransformationById(ID_LEFTLEG_JOINT_MID_ROT)->getValues()[0];
    if (curRotationJointB > maxJointBRotation) {
        jointBRotationUp = false;
    }
    else if (curRotationJointB <= -maxJointBRotation) {
        jointBRotationUp = true;
    }
    
    if (!jointBRotationUp) {
        jointBRotationAmt *= -1;
    }
    
    GLfloat newRotationJointB = curRotationJointB + jointBRotationAmt;
    
    leftJointB->setTransformationValueWithId(ID_LEFTLEG_JOINT_MID_ROT, newRotationJointB, 0, 1.0, 0);
    rightJointB->setTransformationValueWithId(ID_RIGHTLEG_JOINT_MID_ROT, -newRotationJointB, 0, 1.0, 0);
    
}

void Walker::setProjectile(Projectile *p) {
    _projectile = p;
}

Part * Walker::getHead() {
    return _body->getChildren()[0]->getChildren()[0];
}

void Walker::setIsHit() {
    getHead()->setMaterialRedProjectile();
    _hitCount++;
    if (_hitCount > 5) {
        _isDying = true;
    }
}
bool Walker::getIsDying() { return _isDying; }
bool Walker::getIsInactive() { return !_isActive; }

#define SCALE_NECK 0.3
#define SCALE_HEAD 5
#define SCALE_CANNON_ARM 0.04
#define SCALE_CANNON 3

Walker::Walker(GLuint headTextureId, GLuint bodyTextureId,
               Mesh * headMesh, Mesh * cylindarMesh, Mesh * cubeMesh,
               GLfloat * cubeMap, GLfloat * cylinderTextureMap, Cannon * cannon) {
    
    _cannon = cannon;
    _isDying = false;
    _hitCount = 0;
	_animationsAfterBeingHit = 0;
    _angleDegsWrtCannonX = 0.0f;
    _angleDegsWrtCannonY = 0.0f;

    // set a random-ish speed so that not all the walkers move at the same rate
    _speedRandomFactor = 0;
    
    /* make the body parts */
    _body = new Part(bodyTextureId, cubeMap, cubeMesh, -1);
    Part * neck = new Part(bodyTextureId, cylinderTextureMap, cylindarMesh, -1);
    Part * head = new Part(headTextureId, cubeMap, headMesh, -1);
    Part * cannonArm = new Part(bodyTextureId, cylinderTextureMap, cylindarMesh, -1);
    Part * cannonJoint = new Part(bodyTextureId, cylinderTextureMap, cylindarMesh, ID_CANNON_JOINT);
    Part * cannonPart = _cannon->getBody();
    // clear out the default rotations for the Cannon
    cannonPart->Transformations.clear();
    
    /* assemble the parts */
    _body->addChild(neck);
    neck->addChild(head);
    head->addChild(cannonArm);
    cannonArm->addChild(cannonJoint);
    cannonJoint->addChild(cannonPart);
    
    /* orient the parts (rotate, then scale, then translate... generally anyway */
    _body->addRotation(45, 0, 1, 0);
    _body->addRotation(-90, 0, 1, 0);
    _body->addScale(BODY_SCALE, BODY_SCALE*0.1, BODY_SCALE);
    _body->addTranslation(0, WALKER_BODY_HEIGHT, 0, ID_BODY_TRANSLATION);

    neck->addRotation(0, 1, 0, 0, ID_NECK_ROTATION);
    neck->addScale(SCALE_NECK, SCALE_NECK*6, SCALE_NECK);

    preRotate(head);
    head->addScale(SCALE_HEAD, 1.5*SCALE_HEAD, SCALE_HEAD);
    head->addTranslation(0.0, 150.0, 150.0);
    postRotate(head);
    
    preRotate(cannonArm);
    cannonArm->addScale(SCALE_CANNON_ARM, 3*SCALE_CANNON_ARM, SCALE_CANNON_ARM);
    cannonArm->addTranslation(0, -15, 50);
    postRotate(cannonArm);
    
    preRotate(cannonJoint);
    cannonJoint->addRotation(90, 0, 0, 1);
    cannonJoint->addRotation(15, 1, 0, 0, ID_ROTATION_Y);
    cannonJoint->addScale(1, 0.333, 1);
    cannonJoint->addTranslation(80, 0, 0);
    postRotate(cannonJoint);

    preRotate(cannonPart);
    cannonPart->addRotation(90, 1, 0, 0);
    cannonPart->addScale(SCALE_CANNON, SCALE_CANNON, 3*SCALE_CANNON);
    cannonPart->addRotation(0, 0, 1, 0);       // controls vertical angle
    cannonPart->addTranslation(0, 80, -25);
    postRotate(cannonPart);
      
    float legJointAScale = 0.1;
    float legAScale = 1.75;
    float legBScale = 1.25;
    float jointBScale = 1.25;
    
    for(int i = 0; i < 2; i++) {
        
        int topJointId = (i == 0) ? ID_LEFTLEG_JOINT_TOP_ROT : ID_RIGHTLEG_JOINT_TOP_ROT;
        int midJointId = (i == 0) ? ID_LEFTLEG_JOINT_MID_ROT : ID_RIGHTLEG_JOINT_MID_ROT;
        
        /* JOINT A */
        Part * hipjointA = new Part(bodyTextureId, cylinderTextureMap, cylindarMesh, -1);
        _body->addChild(hipjointA);
        preRotate(hipjointA);

        hipjointA->addRotation(90.0, 0.0, 0.0, 1.0);
        hipjointA->addRotation(0, 1.0, 0.0, 0.0, topJointId);  // placeholder for animation
        hipjointA->addScale(legJointAScale, legJointAScale*10, legJointAScale);
        hipjointA->addTranslation(8, 75.0, 0.0); // put in the middle
        float offset = 80;
        if (i == 1) {
            offset *= -1;
        }
        hipjointA->addTranslation(offset, 0.0, 0.0);
        postRotate(hipjointA);
        
        /* leg part A */
        Part * legA = new Part(bodyTextureId, cubeMap, headMesh, -1);
        hipjointA->addChild(legA);
        preRotate(legA);
        legA->addRotation(-90.0, 0.0, 1.0, 0.0);
        legA->addScale(8*legAScale, legAScale, legAScale);
        legA->addRotation(-75.0, 0.0, 1.0, 0.0);
        float legAHeight = -150;
        legA->addTranslation(legAHeight, -60, -450);
        float legAXPos = -120*legAScale;
        if (i == 1) {
            legAXPos *= -1;
        }
        legA->addTranslation(0, legAXPos, 0);
        postRotate(legA);
        
        /* leg joint B */
        Part * jointB = new Part(bodyTextureId, cylinderTextureMap, cylindarMesh, -1);
        legA->addChild(jointB);
        preRotate(jointB);
        jointB->addRotation(0, 0.0, 1.0, 0, midJointId);
        jointB->addScale(jointBScale*0.75, jointBScale, jointBScale * 0.1);
        jointB->addTranslation(0, -50.0, 75.0); // put in the middle
        offset = -20;
        if (i == 1) {
            offset *= -1;
        }
        jointB->addTranslation(0, offset, 0);
        
        postRotate(jointB);
        
        
        /* leg part B */
        Part * legB = new Part(bodyTextureId, cubeMap, headMesh, -1);
        jointB->addChild(legB);
        preRotate(legB);
        legB->addRotation(270.0, 0.0, 1.0, 0.0);
        legB->addScale(10*legBScale, legBScale*0.5, legBScale);
        float legBHeight = -850;
        legB->addTranslation(legBHeight, 40, 0);
        legB->addRotation(-155.0, 0.0, 1.0, 0.0);
        postRotate(legB);

        
        /* foot */
        Part * foot = new Part(bodyTextureId, cubeMap, headMesh, -1);
        legB->addChild(foot);
        
        preRotate(foot);
        float footScale = 1.25;
        foot->addRotation(140.0, 0.0, 1.0, 0.0);
        foot->addScale(footScale*1.5, footScale, footScale*0.15);
        foot->addTranslation(0.0, 0.0, 80.0);
        postRotate(foot);
    }
        
}

Part * Walker::getBody() { return _body; }

