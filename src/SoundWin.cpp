//
//  Sound.cpp
//  cps511a03
//
//  Created by Jeff Mair on 2013-11-07.
//  Copyright (c) 2013 Jeff Mair. All rights reserved.
//

#include "Sound.h"

void Sound::playCannonFire() {
	PlaySound(TEXT("trprsht1.wav"), NULL, SND_FILENAME| SND_ASYNC | SND_NODEFAULT);
}


void Sound::playCannonFireQuiet() {
	PlaySound(TEXT("trprsht1_quiet.wav"), NULL, SND_FILENAME| SND_ASYNC | SND_NODEFAULT);
}

