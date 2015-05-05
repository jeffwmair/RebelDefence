//
//  RenderText.h
//  cps511a1
//
//  Created by Jeff Mair on 2013-10-03.
//
//

#ifndef __cps511a1__RenderText__
#define __cps511a1__RenderText__

#include <iostream>
#include "Shape.h"

class RenderText : public Shape {
private:
    std::string _text;
public:
    RenderText(std::string text, float scale);
    void setText(std::string text);
    std::string getText();
};




#endif /* defined(__cps511a1__RenderText__) */
