/*
 *  Comp.h
 *  cps511a03
 *
 *  Created by Jeff on 13-11-06.
 *  Copyright 2013 Jeff Mair. All rights reserved.
 *
 */

#ifndef __cps511a03__MESH__
#define __cps511a03__MESH__

#include "Mesh.h"
#include "PartTransformation.h"

#define ID_ROTATION_X 100
#define ID_ROTATION_Y 101
#define ID_TRANSLATION 102

class Part {
private:
	
	Mesh * _mesh;
    GLfloat * _textureMap;
    GLuint _textureId;
	std::vector<Part*> _children;
	GLfloat _materialAmbient[4];
    GLfloat _materialSpecular[4];
    GLfloat _materialDiffuse[4];
    GLfloat _materialShininess[1];
    bool _isTextured;
	int _childCount;
    int _id;
    
    bool findChildByIdRecursive(int id, std::vector<Part *> children, Part ** partFound);
	
public:
	Part(GLuint textureId, GLfloat * textureMap, Mesh *m, int partId);
	Part(Mesh *m, int partId);
    Mesh * getMesh();
    bool isVisible;
    
    std::vector<PartTransformation> Transformations;
    PartTransformation * getTransformationById(int id);
    void setTransformationValueWithId(int id, GLfloat val0, GLfloat val1, GLfloat val2, GLfloat val3);
    
    void addRotation(GLfloat angle, GLfloat xvec, GLfloat yvec, GLfloat zvec);
    void addScale(GLfloat x, GLfloat y, GLfloat z);
    void addTranslation(GLfloat x, GLfloat y, GLfloat z);
    
    void addRotation(GLfloat angle, GLfloat xvec, GLfloat yvec, GLfloat zvec, int id);
    void addScale(GLfloat x, GLfloat y, GLfloat z, int id);
    void addTranslation(GLfloat x, GLfloat y, GLfloat z, int id);
    
    GLfloat * getMaterialAmbient();
    GLfloat * getMaterialDiffuse();
    GLfloat * getMaterialSpecular();
    GLfloat * getMaterialShininess();
    GLfloat * getTextureMap();
    
	int getChildCount();
    int getId();
    GLuint getTextureId();
    bool isTextured();
    
    void setMaterialCannon();
    void setMaterialCannonGrooves();
    void setMaterialMetal();
    void setMaterialGreenProjectile();
    void setMaterialGreenProjectileSmall();
    void setMaterialRedProjectile();
    
    void setMaterialAmbient(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
    void setMaterialSpecular(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
    void setMaterialDiffuse(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
    void setMaterialShininess(GLfloat val);
	
	std::vector<Part*> getChildren();
    Part * getChildById(int id);
	void addChild(Part * child);
    
};

#endif

