#ifndef ISCAMERA_H_
#define ISCAMERA_H_
#include <Adapter/UART.h>

#include <Device/Device.h>

#include <Modules/Camera/Commands.h>
#include <Modules/Camera/Types.h>

using ImageProperties = IntroSatLib::camera::types::ImageProperties;
using Chunk = IntroSatLib::camera::types::Chunk;
using Colorspace = IntroSatLib::camera::types::Colorspace;

namespace IntroSatLib {

class Camera: Device {
// public:
//     enum UARTSpeed {
//         US_9600,
//         US_115200,
//         US_230400,
//         US_1000000
//     };

private:

    interfaces::UART* uart;

    template <typename Message>
    ISL_StatusTypeDef receiveMessage(Message* msg);

    template <typename Command>
    ISL_StatusTypeDef sendCommand(Command cmd);
    


public:
    ImageProperties prop;
    Chunk chunk;

    Camera(interfaces::UART _uart);

    ISL_StatusTypeDef Init(uint16_t width, uint16_t height, uint8_t exposure = 0);

    ISL_StatusTypeDef Init() override;

    ISL_StatusTypeDef TakePicture();

    ISL_StatusTypeDef GetImageProperties();

    ISL_StatusTypeDef GetNextChunk();

    ISL_StatusTypeDef SetSize(uint16_t width, uint16_t height);

    ISL_StatusTypeDef SetExposure(uint8_t exposure);
    ISL_StatusTypeDef EnableAutoExposure();



};

} /* namespace IntroSatLib */


#endif /* IS_CAMERA_H_ */