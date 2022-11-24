#include "SSTV.h"
#include "WAV.h"

void addVoxtones(wavBuilder* WAV, int ampl) {
    WAV->addSinFreq(1500, 100, 30000);
    WAV->addSinFreq(1900, 100, 30000);
    WAV->addSinFreq(1500, 100, 30000);
    WAV->addSinFreq(2300, 100, 30000);
    WAV->addSinFreq(1500, 100, 30000);
    WAV->addSinFreq(2300, 100, 30000);
    WAV->addSinFreq(1500, 100, 30000);
}

void addVisCode(wavBuilder* WAV, char visCode, int ampl) {
    WAV->addSinFreq(1900, 300, ampl);
    WAV->addSinFreq(1200, 10, ampl);
    WAV->addSinFreq(1900, 300, ampl);
    WAV->addSinFreq(1200, 30, ampl);

    int bit = 0;
    int ones = 0;
    for (int i = 0; i < 7; i++) {
        bit = (visCode >> i) & 1;
        if (bit == 1) {
            ones++;
            WAV->addSinFreq(1100, 30, ampl); //1
        }
        else {
            WAV->addSinFreq(1300, 30, ampl); //0
        }
    }
    if (ones % 2 == 1) { WAV->addSinFreq(1100, 30, ampl); } //parity 1
    else { WAV->addSinFreq(1300, 30, ampl); } //parity 0
    WAV->addSinFreq(1200, 30, ampl);
}

int jpg_width = 160;
int jpg_height = 120;

//encodes an image into BW8-type SSTV audio
//Input RGBA must be 160x120
//WAV should have >10 seconds allocated
void encodeBW8(wavBuilder* WAV, RGBA* imageData, int signalAmpl) {
    //Add vox tone;
    addVoxtones(WAV, signalAmpl);

    //Add VIS code
    addVisCode(WAV, 0x02, signalAmpl);

    //Add image data
    float mspp = 60.f / (float)jpg_width;
    printf_s("MSPP: %f\n", mspp);

    int pxptr = 0;
    for (int y = 0; y < jpg_height; y++) {
        WAV->addSinFreq(1200, 5, signalAmpl);
        for (int x = 0; x < jpg_width; x++) {
            RGBA c = imageData[pxptr];
            WAV->addSinFreq(1500 + (3.13 * ((c.R + c.G + c.B) / 3)), mspp, signalAmpl);
            pxptr++;
        }
    }
}

void encodeScottie1(wavBuilder* WAV, RGBA* rgbBuffer, int signalAmpl) {
    //Add vox tone;
    addVoxtones(WAV, signalAmpl);

    //Add VIS code
    addVisCode(WAV, 0x3C, signalAmpl);

    float mspp = 138.240 / (float)jpg_width;
    int pxptr = 0;
    WAV->addSinFreq(1200, 9, signalAmpl);
    for (int y = 0; y < jpg_height; y++) {
        int tmpPxptr = pxptr;

        for (int x = 0; x < jpg_width; x++) {
            RGBA c = rgbBuffer[pxptr];
            WAV->addSinFreq(1500 + (3.13 * c.G), 0.4320, signalAmpl);
            pxptr++;
        }
        pxptr = tmpPxptr;

        WAV->addSinFreq(1500, 1.5, signalAmpl);
        for (int x = 0; x < jpg_width; x++) {
            RGBA c = rgbBuffer[pxptr];
            WAV->addSinFreq(1500 + (3.13 * c.B), 0.4320, signalAmpl);
            pxptr++;
        }
        pxptr = tmpPxptr;

        WAV->addSinFreq(1200, 9, signalAmpl);
        WAV->addSinFreq(1500, 1.5, signalAmpl);
        for (int x = 0; x < jpg_width; x++) {
            RGBA c = rgbBuffer[pxptr];
            WAV->addSinFreq(1500 + (3.13 * c.R), 0.4320, signalAmpl);
            pxptr++;
        }
    }
}