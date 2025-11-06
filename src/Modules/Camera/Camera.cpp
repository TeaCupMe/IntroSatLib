#define INTROSATLIB_INTERNAL
#include <Modules/Camera.h>

namespace IntroSatLib {

Camera::Camera(interfaces::UART _uart): uart(new interfaces::UART(_uart)) {}

ISL_StatusTypeDef Camera::Init() {
    return Init(640, 480, 0);
}

ISL_StatusTypeDef Camera::Init(uint16_t width, uint16_t height, uint8_t exposure) {
    ISL_StatusTypeDef status;
    status = SetExposure(exposure);
    if (status != ISL_OK) return status;

    status = SetSize(width, height);
    if (status != ISL_OK) return status;
    
    return ISL_OK;
}

ISL_StatusTypeDef Camera::TakePicture() {
    ISL_StatusTypeDef status;
    status = sendCommand(camera::commands::CMD_TAKE_PICTURE);
    if (status != ISL_OK) return status;

    status = uart->waitForPattern((uint8_t*) camera::commands::MESSAGE_START, 3, 2000);
    if (status != ISL_OK) return status;

    status = uart->waitForPattern((uint8_t*) camera::commands::MESSAGE_END, 3, 300);
    if (status != ISL_OK) return status;

    return ISL_OK;
}

ISL_StatusTypeDef Camera::GetImageProperties() {
    ISL_StatusTypeDef status;
    status = sendCommand(camera::commands::CMD_GET_IMAGE_PROPERTIES);
    if (status != ISL_OK) return status;

    return receiveMessage(&prop);
}

ISL_StatusTypeDef Camera::GetNextChunk() {
    ISL_StatusTypeDef status;
    status = sendCommand(camera::commands::CMD_GET_NEXT_CHUNK);
    if (status != ISL_OK) return status;

    return receiveMessage(&chunk);
}

ISL_StatusTypeDef Camera::SetSize(uint16_t width, uint16_t height) {
    camera::commands::CommadSetSize cmd;
    cmd.width = width;
    cmd.height = height;
    return sendCommand(cmd);
}

ISL_StatusTypeDef Camera::SetExposure(uint8_t exposure) {
    camera::commands::CommandSetExposure cmd;
    cmd.exposure = exposure;
    return sendCommand(cmd);
}

ISL_StatusTypeDef Camera::EnableAutoExposure() {
    SetExposure(0);
}

template <typename Command>
ISL_StatusTypeDef Camera::sendCommand(Command cmd) {
    return uart->transmit((uint8_t*) &cmd, sizeof(Command));
}

template <typename Message>
ISL_StatusTypeDef Camera::receiveMessage(Message* msg) {
    ISL_StatusTypeDef status;

    status = uart->waitForPattern((uint8_t*) camera::commands::MESSAGE_START, 3, 2000);
    if (status != ISL_OK) return status;

    status = uart->receive((uint8_t*) msg, sizeof(Message));
    if (status != ISL_OK) return status;

    status = uart->waitForPattern((uint8_t*) camera::commands::MESSAGE_END, 3, 300);
    if (status != ISL_OK) return status;
}

}; /* namespace IntroSatLib */