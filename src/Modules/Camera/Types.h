#ifndef MODULES_CAMERA_TYPES_H_
#define MODULES_CAMERA_TYPES_H_

namespace IntroSatLib::camera::types
{
    enum Colorspace {
        // EMPTY,
        // RGB565,
        // RGB555,
        // RGB444,
        // YUV,
        WB // TODO: Modified YUV???
    };

    struct ImageProperties {
        uint16_t height, width;
        uint16_t vStart, hStart;
        Colorspace colorspace = WB;
        uint16_t exposure;
        uint32_t length;
        uint16_t numberOfChunks;
    };

    struct Chunk {
        uint16_t chunkID, payloadLength;
        bool isLastChunk;
        uint8_t payload[240];
        uint8_t checksum;
    };
}

#endif /* MODULES_CAMERA_TYPES_H_ */