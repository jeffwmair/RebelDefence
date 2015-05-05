//
//  MyMath.h
//  cps511a2
//
//  Created by Jeff Mair on 2013-11-02.
//  Copyright (c) 2013 Jeff Mair. All rights reserved.
//

#include <math.h>

#ifndef cps511a2_MyMath_h
#define cps511a2_MyMath_h

void getCrossProduct(float a1, float a2, float a3, float b1, float b2, float b3, float normal[3]) {
        
    // normalize
    float aSum = sqrt(powf(a1,2.0) + powf(a2,2.0) + powf(a3,2.0));
    a1 = a1 / aSum;
    a2 = a2 / aSum;
    a3 = a3 / aSum;
    
    // normalize
    float bSum = sqrt(powf(b1,2.0) + powf(b2,2.0) + powf(b3,2.0));
    b1 = b1 / bSum;
    b2 = b2 / bSum;
    b3 = b3 / bSum;
    
    normal[0] = a2*b3 - a3*b2;
    normal[1] = a3*b1 - a1*b3;
    normal[2] = a1*b2 - a2*b1;
    
}

#endif
