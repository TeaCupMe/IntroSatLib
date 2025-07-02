#ifndef IS_BLUETOOTH_H_
#define IS_BLUETOOTH_H_

#include <stdint.h>



#ifdef STM32F4
    #include "stm32f4xx_hal.h"
    #define BOOT_ADDR 0x1FFFF000
#endif

#ifdef STM32F1
    #include "stm32f1xx_hal.h"
    #define BOOT_ADDR 0x1FFFF000
#endif

#ifdef STM32H750xx
    #include "stm32h7xx_hal.h"
    #define BOOT_ADDR 0x1FF09800
#endif

#ifdef BOOT_ADDR
    #define BOOTVTAB	((struct boot_vectable_ *)BOOT_ADDR)
#else
    // Looks like unsupported platform
    #warning "IntroSatLib::EnterBootloader() not supported by selected core"
    #warning "IntroSatLib::EnterBootloader() currently supports STM32F1, STM32F4, STM32H750xx."
    #warning "Calls to IntroSatLib::EnterBootloader() will have no effect"
#endif




#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern void EnterBootloader();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* IS_BLUETOOTH_H_ */
