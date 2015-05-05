//
//  Sound.h
//  cps511a03
//
//  Created by Jeff Mair on 2013-11-07.
//  Copyright (c) 2013 Jeff Mair. All rights reserved.
//

#ifndef __cps511a03__Sound__
#define __cps511a03__Sound__

#ifdef __APPLE__
#include <AudioToolbox/AudioToolbox.h>
#include <CoreServices/CoreServices.h>
#elif defined _WIN32 || defined _WIN64
#include <Windows.h>
#include <MMSystem.h>
#include <string>
#endif


#include <iostream>

class Sound {
public:
    static void playCannonFire();
    static void playCannonFireQuiet();
};

#endif /* defined(__cps511a03__Sound__) */
