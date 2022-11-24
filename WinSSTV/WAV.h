#pragma once
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#define PI 3.1415926535

struct monoSample {
    short S;
};

struct stereoSample {
    short L;
    short R;
};

struct wavHeader {
    char riff[4] = { 'R', 'I', 'F', 'F' };
    int fileSize = 44;
    char wave[4] = { 'W', 'A', 'V', 'E' };
    char fmt[4] = { 'f', 'm', 't', 0x20 };
    int headerSize = 16;
    short format = 1;
    short channels = 1;
    int sampleRate = 48000;
    int SBC = 96000;
    short BC = 2;
    short BPS = 16;
    char data[4] = { 'd', 'a', 't', 'a' };
    int dataSize = 0;
};

class wavBuilder {
private:
    wavHeader header = {};
    short* samples;

    float sinAngle = 0.f;
    int currentSamplePointer = 0;
    int bytesWritten = 0;

public:
    wavBuilder(int sampleRate, int initialTimeAlloc) {
        header.sampleRate = sampleRate;
        header.SBC = (header.sampleRate * header.BPS * header.channels) / 8;
        header.dataSize = sizeof(short) * (header.sampleRate * initialTimeAlloc);
        header.fileSize = sizeof(wavHeader) + header.dataSize;
        samples = (short*)malloc(header.dataSize);
    };

    void* finish(bool dispose);
    void addSinFreq(short frequency, float duration, int amplitude);
    int fileSize();
};