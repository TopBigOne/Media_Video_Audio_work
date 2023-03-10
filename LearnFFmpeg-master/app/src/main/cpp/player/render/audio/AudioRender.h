//
// Created by dev on 2023/2/21.
//

#ifndef LEARNFFMPEG_AUDIORENDER_H
#define LEARNFFMPEG_AUDIORENDER_H

#include <stdint.h>
#include <stdlib.h>
#include <cstring>

class AudioFrame {
public:
    uint8_t *data = nullptr;
    int dataSize = 0;
    bool hardCopy = true;

    AudioFrame(uint8_t *data, int dataSize, bool hardCopy = true) {
        this->data = data;
        this->dataSize = dataSize;
        if (hardCopy) {
            this->data = static_cast<uint8_t * >(malloc(this->dataSize));
            memcpy(this->data, data, dataSize);
        }
    }

    ~AudioFrame() {
        if (hardCopy && this->data) {
            free(this->data);
            this->data = nullptr;
        }
    }
};

class AudioRender {
public:
    virtual ~AudioRender() {}

    virtual void Init() = 0;

    virtual void ClearAudioCache() = 0;

    virtual void RenderAudioFrame(uint8_t *pData, int dataSize) = 0;

    virtual void UnInit() = 0;

};


#endif //LEARNFFMPEG_AUDIORENDER_H
