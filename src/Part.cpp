 /*
 *  Comp.cpp
 *  cps511a03
 *
 *  Created by Jeff on 13-11-06.
 *  Copyright 2013 Jeff Mair. All rights reserved.
 *
 */

#include "Part.h"



Part::Part(GLuint textureId, GLfloat * textureMap, Mesh *m, int partId) {

    _id = partId;
	_mesh = m;
    _textureId = textureId;
    _textureMap = textureMap;
    isVisible = true;
    _isTextured = true;
	_childCount = 0;
}

Part::Part(Mesh * m, int partId) {
    
    _id = partId;
	_textureId = -1;
    _isTextured = false;
    _mesh = m;
    _textureId = -1;
    isVisible = true;
	_childCount = 0;
}

int Part::getId() { return _id; }

bool Part::findChildByIdRecursive(int id, std::vector<Part *> children, Part ** partFound) {
    for (int i = 0; i < children.size(); i++) {
        Part * p = children[i];
        if (p->getChildCount() > 0) {
            if (findChildByIdRecursive(id, p->getChildren(), partFound)) {
                return true;
            }
        }
        
        if (p->getId() == id) {
            *partFound = p;
            return true;
        }
    }
    
    
    return false;
}

Part * Part::getChildById(int id) {
    Part * p;
    if (!findChildByIdRecursive(id, getChildren(), &p)) {
        std::cout << "Could not find the part with id " << id << std::endl;
    }
    return p;
}

GLfloat Part::getTransformationById(int id, int index) {
    for (int i = 0; i < Transformations.size(); i++) {
        PartTransformation pt = Transformations[i];
        if (pt.getId() == id) {
            return pt.getValues()[index];
        }
    }
}

void Part::setTransformationValueWithId(int id, GLfloat val0, GLfloat val1, GLfloat val2, GLfloat val3) {
    for (int i = 0; i < Transformations.size(); i++) {
        PartTransformation this_pt = Transformations[i];
        if (this_pt.getId() == id) {
            Transformations[i] = PartTransformation(this_pt.getOpType(), val0, val1, val2, val3, id);
        }
    }
}

void Part::addRotation(GLfloat val0, GLfloat val1, GLfloat val2, GLfloat val3) {
    PartTransformation po(OP_ROTATE, val0, val1, val2, val3, 0);
    Transformations.push_back(po);    
}
void Part::addScale(GLfloat x, GLfloat y, GLfloat z) {
    PartTransformation po(OP_SCALE, x, y, z, 0, 0);
    Transformations.push_back(po);
}

void Part::addTranslation(GLfloat x, GLfloat y, GLfloat z) {
    PartTransformation po(OP_TRANSLATE, x, y, z, 0, 0);
    Transformations.push_back(po);
}

void Part::addRotation(GLfloat val0, GLfloat val1, GLfloat val2, GLfloat val3, int id) {
    PartTransformation po(OP_ROTATE, val0, val1, val2, val3, id);
    Transformations.push_back(po);
}
void Part::addScale(GLfloat x, GLfloat y, GLfloat z, int id) {
    PartTransformation po(OP_SCALE, x, y, z, 0, id);
    Transformations.push_back(po);
}

void Part::addTranslation(GLfloat x, GLfloat y, GLfloat z, int id) {
    PartTransformation po(OP_TRANSLATE, x, y, z, 0, id);
    Transformations.push_back(po);
}

bool Part::isTextured() { return _isTextured; }
GLfloat * Part::getTextureMap() { return _textureMap; }


int Part::getChildCount() {	return _childCount; }
GLuint Part::getTextureId() { return _textureId; }
std::vector<Part*> Part::getChildren() { return _children; }

void Part::addChild(Part * child) {
	_children.push_back(child);
	_childCount++;
}

GLfloat * Part::getMaterialAmbient() { return _materialAmbient; }
GLfloat * Part::getMaterialSpecular() { return _materialSpecular; }
GLfloat * Part::getMaterialDiffuse() { return _materialDiffuse; }
GLfloat * Part::getMaterialShininess() { return _materialShininess; }

void Part::setMaterialAmbient(GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
    _materialAmbient[0] = x;
    _materialAmbient[1] = y;
    _materialAmbient[2] = z;
    _materialAmbient[3] = w;
}
void Part::setMaterialSpecular(GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
    _materialSpecular[0] = x;
    _materialSpecular[1] = y;
    _materialSpecular[2] = z;
    _materialSpecular[3] = w;
}
void Part::setMaterialDiffuse(GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
    _materialDiffuse[0] = x;
    _materialDiffuse[1] = y;
    _materialDiffuse[2] = z;
    _materialDiffuse[3] = w;
}
void Part::setMaterialShininess(GLfloat val) {
    _materialShininess[0] = val;
}

void Part::setMaterialMetal() {
    setMaterialAmbient(0.4, 0.4, 0.4, 1.0);
    setMaterialSpecular(0.9, 0.9,0.9, 1.0);
    setMaterialDiffuse(0.7, 0.7, 0.7, 1.0);
    setMaterialShininess(0.5);
}

void Part::setMaterialCannon() {
    setMaterialAmbient(0.5, 0.5, 0.5, 1.0);
    setMaterialSpecular(0.1, 0.1, 0.1, 1.0);
    setMaterialDiffuse(0.4, 0.4, 0.4, 1.0);
    setMaterialShininess(0.05);
}

void Part::setMaterialCannonGrooves() {
    setMaterialAmbient(0.1, 0.1, 0.1, 1.0);
    setMaterialSpecular(0.05, 0.05, 0.05, 1.0);
    setMaterialDiffuse(0.025, 0.025, 0.025, 1.0);
    setMaterialShininess(0.1);
}

void Part::setMaterialGreenProjectile() {
    setMaterialAmbient(0.2, 8, 0.2, 1.0);
    setMaterialSpecular(0.2, 8, 0.2, 1.0);
    setMaterialDiffuse(0.2, 8, 0.2, 1.0);
    setMaterialShininess(0.8);
}
void Part::setMaterialGreenProjectileSmall() {
    
    float r = 0.3, g = 0.8, b = 0.2;
    setMaterialAmbient(r, g, b, 1.0);
    setMaterialSpecular(r, g, b, 1.0);
    setMaterialDiffuse(r, g, b, 1.0);
    setMaterialShininess(0.6);
}
void Part::setMaterialRedProjectile() {
    setMaterialAmbient(8, 0.1, 0.1, 1.0);
    setMaterialSpecular(8, 0.1, 0.2, 1.0);
    setMaterialDiffuse(8, 0.1, 0.2, 1.0);
    setMaterialShininess(0.8);
}

Mesh * Part::getMesh() { return _mesh; }


