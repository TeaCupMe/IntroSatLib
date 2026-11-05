/**
 * @file BusOp.h
 * @brief Operation tags recorded by host GPIO/SPI/UART/System stubs via CallLog.
 */
#ifndef INTROSATLIB_TEST_BUS_OP_H_
#define INTROSATLIB_TEST_BUS_OP_H_

#include <cstdint>

namespace IntroSatLib {
namespace test {

enum class BusOp : uint8_t {
	SPI_Transfer,
	GPIO_Read,
	GPIO_Write,
	GPIO_IsValid,
	GPIO_Tone,
	GPIO_NoTone,
	System_GetTick,
	System_GetMicros,
	System_Delay,
	System_DelayMicroseconds,
	UART_Available,
	UART_Receive,
	UART_Transmit,
	UART_TransmitAsync,
};

} /* namespace test */
} /* namespace IntroSatLib */

#endif /* INTROSATLIB_TEST_BUS_OP_H_ */
