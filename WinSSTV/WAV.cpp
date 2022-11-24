#include "WAV.h"

void wavBuilder::addSinFreq(short frequency, float duration, int amplitude) {
    monoSample* wavData = (monoSample*)(this->samples);
    int sampleCount = (this->header.sampleRate) * (duration / 1000.f);
    for (int i = 0; i < sampleCount; i++) {
        bytesWritten += (int)sizeof(short);

        wavData[this->currentSamplePointer] = { (short)(amplitude * sin(this->sinAngle)) };
        this->sinAngle += ((2 * PI * frequency) / header.sampleRate);

        this->currentSamplePointer++;
        while (this->sinAngle > 2 * PI) { this->sinAngle -= 2 * PI; } //avoid floating point weirdness
    }
}

void* wavBuilder::finish(bool dispose) {
    this->header.dataSize -= this->header.dataSize - this->bytesWritten;
    this->header.fileSize = this->header.dataSize + sizeof(wavHeader);

    void* completeWav = malloc(this->header.fileSize);
    memcpy_s(completeWav, this->header.fileSize, &this->header, sizeof(wavHeader));
    memcpy_s((void*)((uintptr_t)completeWav + sizeof(wavHeader)), this->header.fileSize, this->samples, this->header.dataSize);
    if (dispose) { free(this->samples); }

    return completeWav;
}

int wavBuilder::fileSize() {
    return this->header.fileSize;
}