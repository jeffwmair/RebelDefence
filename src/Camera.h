//
//  Camera.h
//  cps511a2
//
//  Created by Jeff Mair on 2013-11-01.
//  Copyright (c) 2013 Jeff Mair. All rights reserved.
//

#ifndef __cps511a2__Camera__
#define __cps511a2__Camera__

#ifdef __APPLE__
#include <GLUT/glut.h>
#elif defined _WIN32 || defined _WIN64
#include <stdlib.h>
#include <glut.h>
#endif

#ifdef __linux__
#include <GL/glut.h>
#endif


#define ZOOM_INCREMENT 250
#define CAMERA_ANGLE_CHANGE_AMOUNT 0.5
#define CAM_MAX_VIEW_DISTANCE 50000.0
#define CAM_MIN_VIEW_DISTANCE 1.0
#define CAM_ASPECT_RATIO_X 1.0

#define A_PI 3.1415926

#include <math.h>


class Camera {
private:
    GLfloat _camX, _camY, _camZ, _zoom, _upVecY, _upVecZ, _angleH, _angleV;
    void updateAngle();
public:
    
	Camera(float initialDistance, float initialVAngle, float initialHangle);
    void adjustVerticalAngle(float degreeChange);
    void adjustHorizontalAngle(float degreeChange);
    void setVAngle(float newAngle);
    void setHAngle(float newAngle);
    
    void zoomIn();
    void zoomOut();

    GLfloat getCamX();
    GLfloat getCamY();
    GLfloat getCamZ();
    GLfloat getCamUpY();
    GLfloat getCamUpZ();
    GLfloat getZoom();
    
    GLfloat getHAngle();
    GLfloat getVAngle();
    
};

#endif /* defined(__cps511a2__Camera__) */
