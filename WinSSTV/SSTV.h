#pragma once
struct RGBA {
	unsigned char R;
	unsigned char G;
	unsigned char B;
	unsigned char A;
};

#include "SSTV.h"
#include "WAV.h"

//encodes an image into BW8-type SSTV audio
//Input RGBA must be 160x120
//WAV should have ~15 seconds allocated
void encodeBW8(wavBuilder* WAV, RGBA* imageData, int signalAmpl = 30000);


//encodes an image into Scottie1-type SSTV audio
//Input RGBA must be 320x240
//WAV should have ~160 seconds allocated
void encodeScottie1(wavBuilder* WAV, RGBA* rgbBuffer, int signalAmpl = 30000)