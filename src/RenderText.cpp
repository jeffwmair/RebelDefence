//
//  RenderText.cpp
//  cps511a1
//
//  Created by Jeff Mair on 2013-10-03.
//
//

#include <iostream>
#include "RenderText.h"

using namespace std;

RenderText::RenderText(string text, float scale) {
    _text = text;
    setScaling(scale, scale, scale);
}

void RenderText::setText(std::string text) {
    _text = text;
}

string RenderText::getText() { return _text; }