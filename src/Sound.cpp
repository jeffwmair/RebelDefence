//
//  Sound.cpp
//  cps511a03
//
//  Created by Jeff Mair on 2013-11-07.
//  Copyright (c) 2013 Jeff Mair. All rights reserved.
//

#include "Sound.h"

void playSound(char * filePath) {
    CFURLRef audioFileURL = CFURLCreateFromFileSystemRepresentation (NULL,
                                                                     (const UInt8 *) filePath,
                                                                     strlen (filePath),
                                                                     false
                                                                     );
    
    SystemSoundID soundFileObject;
    OSStatus result = AudioServicesCreateSystemSoundID(audioFileURL, &soundFileObject);
    CFRelease (audioFileURL);
    AudioServicesPlaySystemSound(soundFileObject);
}

void Sound::playCannonFire() {
    playSound("trprsht1.wav");
}


void Sound::playCannonFireQuiet() {
    playSound("trprsht1_quiet.wav");
}

