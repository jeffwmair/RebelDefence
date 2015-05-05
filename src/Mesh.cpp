//
//  World.cpp
//  cps511a1
//
//  Created by Jeff Mair on 2013-09-23.
//
//

#include "Mesh.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>

using namespace std;

void Mesh::setTotalNumVerticies(int num) {
    _numVertices = num;
}

int Mesh::getNumVerticiesTotal() {
    return _numVertices;
}


GLfloat * Mesh::getNormals() {
	return _normalsAll;
}

GLfloat * Mesh::getVerticies() {
	return _verticiesAll;
}


