//
//  Camera.cpp
//  cps511a2
//
//  Created by Jeff Mair on 2013-11-01.
//  Copyright (c) 2013 Jeff Mair. All rights reserved.
//

#include "Camera.h"
#include <iostream>

void getCrossProduct(float a1, float a2, float a3, float b1, float b2, float b3, float normal[3]) ;

Camera::Camera(float initialDistance, float initialVAngle, float initialHangle) {
    _upVecY = 1.0;
    _upVecZ = 0.0;
    _zoom = initialDistance;
    _angleH = initialHangle;
    _angleV = initialVAngle;
    updateAngle();
}

void Camera::adjustVerticalAngle(float degreeChange) {
    _angleV += degreeChange;
    updateAngle();
}
void Camera::adjustHorizontalAngle(float degreeChange) {
    _angleH += degreeChange;
    updateAngle();
}

void Camera::setVAngle(float angle)  {
    _angleV = angle;
    updateAngle();
}

void Camera::setHAngle(float angle) {
    _angleH = angle;
    updateAngle();
}

GLfloat Camera::getHAngle() {
    return _angleH;
}

GLfloat Camera::getVAngle() {
    return _angleV;
}

void Camera::updateAngle() {
    
    _angleV = fmod(_angleV, 360.0f);
    _angleH = fmod(_angleH, 360.0f);
    
    float angleVRad = A_PI * _angleV / 180.0;
    float angleHRad = A_PI * _angleH / 180.0;
    float angleHDummyForUpVect = A_PI * (_angleH + 2) / 180.0;

    _camZ = _zoom * sin(angleVRad) * cos(angleHRad);
    _camX = _zoom * sin(angleVRad) * sin(angleHRad);
    _camY = _zoom * cos(angleVRad);

    glutPostRedisplay();
}

void Camera::zoomIn() {
    _zoom -= ZOOM_INCREMENT;
    updateAngle();
}
void Camera::zoomOut() {
    _zoom += ZOOM_INCREMENT;
    updateAngle();
}


GLfloat Camera::getCamX() { return _camX; }
GLfloat Camera::getCamY() { return _camY; }
GLfloat Camera::getCamZ() { return _camZ; }
GLfloat Camera::getCamUpY() { return _upVecY; }
GLfloat Camera::getCamUpZ() { return _upVecZ; }
GLfloat Camera::getZoom() { return _zoom; }
