#include "IRCamera.h"

namespace IntroSatLib {

//#ifndef ARDUINO
IRCamera::IRCamera(const interfaces::I2C &i2c, uint8_t address): I2CDevice(new interfaces::I2C(i2c), address)
{
}
//#else
//IRCamera::IRCamera(TwoWire &hi2c, uint8_t address): BaseDevice(hi2c, address)
//{
//}
//IRCamera::IRCamera(uint8_t address): BaseDevice(address)
//{
//}
//#endif

IRCamera::IRCamera(const IRCamera &other): I2CDevice(other)
{
	_framrate = other._framrate;
	_resetPort = other._resetPort;
	_resetPin = other._resetPin;
	_mirror = other._mirror;
}
IRCamera::IRCamera(IRCamera &&other): I2CDevice(other)
{
	_framrate = other._framrate;
	_resetPort = other._resetPort;
	_resetPin = other._resetPin;
	_mirror = other._mirror;
}
IRCamera& IRCamera::operator=(const IRCamera &other)
{
	if (this != &other)
	{
		this->I2CDevice::operator = (other);
		_framrate = other._framrate;
		_resetPort = other._resetPort;
		_resetPin = other._resetPin;
		_mirror = other._mirror;
	}
	return *this;
}
IRCamera& IRCamera::operator=(IRCamera &&other)
{
	if (this != &other)
	{
		this->I2CDevice::operator = (other);
		_framrate = other._framrate;
		_resetPort = other._resetPort;
		_resetPin = other._resetPin;
		_mirror = other._mirror;
	}
	return *this;
}

ISL_StatusTypeDef IRCamera::Init(Framerate framrate)
{
	tryReset();
	ISL_StatusTypeDef status = ISL_StatusTypeDef::ISL_OK;

	if ((status = IsReady()) != 0) return status;

	uint8_t value = 0;
	if ((status = SetRegisterI2C(AMG88xx_PCTL, &value, 1)) != 0) { return status; }

	value = 0x3F;
	if ((status = SetRegisterI2C(AMG88xx_RST, &value, 1)) != 0) { return status; }

	value = 0;
	if ((status = SetRegisterI2C(AMG88xx_INTC, &value, 1)) != 0) { return status; }

	value = 0x01;
	if ((status = SetRegisterI2C(AMG88xx_FPSC, &value, 1)) != 0) { return status; }

	HAL_Delay(1000);
	return status;
}

ISL_StatusTypeDef IRCamera::Init() { return Init(Framerate::FPS_10); }

int16_t IRCamera::int12ToInt16(uint16_t val)
{
	int16_t sVal = (val << 4);
	return sVal >> 4;
}

uint8_t IRCamera::Read()
{
	uint8_t buffer[128];

	ISL_StatusTypeDef status;
	if ((status = ReadRegisterI2C(AMG88xx_PIXEL_OFFSET, buffer, 128)) != 0) { return status; }

	for (int i = 0; i < 64; i++)
	{
		uint8_t pos = i << 1;
		uint16_t recast = ((uint16_t)buffer[pos + 1] << 8) | ((uint16_t)buffer[pos]);

		_buffer[_mirror ? 63 - i : i] = int12ToInt16(recast);
	}
	return 0;
}

int16_t IRCamera::getPixelRaw(uint8_t x, uint8_t y)
{
	return _buffer[((y & 7) << 3) + (x & 7)];
}

float IRCamera::getPixel(uint8_t x, uint8_t y)
{
	return ((float)getPixelRaw(x, y)) * _rawdeg;
}

void IRCamera::useForceReset(GPIO_TypeDef* resetPort, uint16_t resetPin)
{
	_resetPort = resetPort;
	_resetPin = resetPin;
}

void IRCamera::useMirrored() { _mirror = 1; }

void IRCamera::useNotMirrored() { _mirror = 0; }


void IRCamera::tryReset()
{
	if (_resetPort == 0) { return; }
	HAL_GPIO_WritePin(_resetPort, _resetPin, GPIO_PIN_SET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(_resetPort, _resetPin, GPIO_PIN_RESET);
	HAL_Delay(100);
}

IRCamera::~IRCamera() { }

}
