#ifndef MODULES_CAMERA_COMMANDS_H_
#define MODULES_CAMERA_COMMANDS_H_
#include <Modules/Camera/Types.h>

namespace IntroSatLib::camera::commands
{
    const uint8_t MESSAGE_START[3] = {0xFF, 0xFF, 0x00};
	const uint8_t MESSAGE_END[3] = {0x00, 0xFF, 0x00};

    const uint8_t CMD_TAKE_PICTURE = 0x74;          // 't'

    const uint8_t CMD_GET_IMAGE_PROPERTIES = 0x70;  // 'p'
    // const uint8_t CMD_RESET_CHUNK_COUNTER = 0x71;   // 'r'
    const uint8_t CMD_GET_NEXT_CHUNK = 0x6e;        // 'n'
    // const uint8_t CMD_GET_CHUNK_BY_ID = 'i';
    // const uint8_t CMD_DUMP_IMAGE = 'f';

    const uint8_t CMD_SET_SIZE = 0x73;              // 's'
    const uint8_t CMD_SET_EXPOSURE = 0x65;          // 'e'
    // const uint8_t CMD_SET_COLORSPACE = 0x22;        

    template <uint8_t CMD, typename responseType = void>
    struct Command {
        uint8_t cmd = CMD;
        size_t getResponseSize() {
            return sizeof(responseType);
        }
    };

    struct CommandTakePicture: Command<CMD_TAKE_PICTURE> {
        // uint8_t cmd = 0x74;
    };

    struct CommandGetImageProperties: Command<CMD_GET_IMAGE_PROPERTIES> {
        
    };

    struct CommandGetNextChunk: Command<CMD_GET_NEXT_CHUNK, camera::types::Chunk> {

    };

    struct CommadSetSize: Command<CMD_SET_SIZE> {
        uint16_t width;
        uint16_t height;
    };

    struct CommandSetExposure: Command<CMD_SET_EXPOSURE> {
        uint8_t exposure;
    };
}

#endif /* MODULES_CAMERA_COMMANDS_H_ */