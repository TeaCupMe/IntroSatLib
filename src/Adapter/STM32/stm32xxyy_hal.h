/*
 * stm32XXyy_hal.h
 *
 *  Created on: Jul 5, 2025
 *      Author: alesh
 */

#ifndef INTERFACE_STM32_STM32XXYY_HAL_H_
#define INTERFACE_STM32_STM32XXYY_HAL_H_

// \n[^\n]*\n#elif defined\(STM32H7([^\)]*)\) ->  || defined(STM32H7$1)

// STM32C0 Series
#if defined(STM32C011xx) || defined(STM32C031xx) || defined(STM32C051xx) || defined(STM32C071xx) || defined(STM32C091xx) || defined(STM32C092xx)
	#include "stm32c0xx_hal.h"

#if defined(STM32F030x6) || defined(STM32F030x8) || defined(STM32F030xC) ||\
	defined(STM32F031x6) || defined(STM32F038xx) || defined(STM32F042x6) ||\
	defined(STM32F048xx) || defined(STM32F051x8) || defined(STM32F058xx) ||\
	defined(STM32F070x6) || defined(STM32F070xB) || defined(STM32F071xB) ||\
	defined(STM32F072xB) || defined(STM32F078xx) || defined(STM32F091xC) ||\
	defined(STM32F098xx)
	#include "stm32f0xx_hal.h"


#if defined(STM32F100xB) || defined(STM32F100xE) || defined(STM32F101x6) ||\
	defined(STM32F101xB) || defined(STM32F101xE) || defined(STM32F101xG) ||\
	defined(STM32F102x6) || defined(STM32F102xB) || defined(STM32F103x6) ||\
	defined(STM32F103xE) || defined(STM32F103xE) || defined(STM32F103xG) ||\
	defined(STM32F105xC) || defined(STM32F107xC)
	#include "stm32f1xx_hal.h"


#if defined(STM32F205xx) || defined(STM32F207xx) || defined(STM32F215xx) ||\
	defined(STM32F217xx)
	#include "stm32f2xx_hal.h"


#if defined(STM32F301x8) || defined(STM32F302x8) || defined(STM32F302xC) ||\
	defined(STM32F302xE) || defined(STM32F303x8) || defined(STM32F303xC) ||\
	defined(STM32F303xE) || defined(STM32F318xx) || defined(STM32F328xx) ||\
	defined(STM32F334x8) || defined(STM32F358xx) || defined(STM32F373xC) ||\
	defined(STM32F378xx) || defined(STM32F398xx)
	#include "stm32f3xx_hal.h"


#if defined(STM32F401xC) || defined(STM32F401xE) || defined(STM32F405xx) || defined(STM32F407xx) || defined(STM32F410Cx) || defined(STM32F410Rx) || defined(STM32F410Tx) || defined(STM32F411xE) || defined(STM32F412Cx) || defined(STM32F412Rx) || defined(STM32F412Vx) || defined(STM32F412Zx) || defined(STM32F413xx) || defined(STM32F415xx) || defined(STM32F417xx) || defined(STM32F423xx) || defined(STM32F427xx) || defined(STM32F429xx) || defined(STM32F437xx) || defined(STM32F439xx) || defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx)
	#include "stm32f4xx_hal.h"

#if defined(STM32F722xx) || defined(STM32F723xx) || defined(STM32F730xx) || defined(STM32F732xx) || defined(STM32F733xx) || defined(STM32F745xx) || defined(STM32F746xx) || defined(STM32F750xx) || defined(STM32F756xx) || defined(STM32F765xx) || defined(STM32F767xx) || defined(STM32F769xx) || defined(STM32F777xx) || defined(STM32F779xx)
	#include "stm32f7xx_hal.h"

#if defined(STM32G030xx) || defined(STM32G031xx) || defined(STM32G041xx) || defined(STM32G050xx) || defined(STM32G051xx) || defined(STM32G061xx) || defined(STM32G070xx) || defined(STM32G071xx) || defined(STM32G081xx) || defined(STM32G0B0xx) || defined(STM32G0B1xx) || defined(STM32G0C1xx)
	#include "stm32g0xx_hal.h"

#if defined(STM32G411xB) || defined(STM32G411xC) || defined(STM32G414xx) || defined(STM32G431xx) || defined(STM32G441xx) || defined(STM32G471xx) || defined(STM32G473xx) || defined(STM32G474xx) || defined(STM32G483xx) || defined(STM32G484xx) || defined(STM32G491xx) || defined(STM32G4A1xx)
	#include "stm32g4xx_hal.h"

#elif defined(STM32GBK1CB)
	#error "Unsupported?"
	#include "stm32gbk1cb_hal.h"

#if defined(STM32H503xx) || defined(STM32H523xx) || defined(STM32H533xx) || defined(STM32H562xx) || defined(STM32H563xx) || defined(STM32H573xx)
	#include "stm32h5xx_hal.h"

#elif defined(STM32H723xx)
	#include "stm32h7xx_hal.h"
#elif defined(STM32H725xx)
	#include "stm32h7xx_hal.h"
#elif defined(STM32H730xx)
	#include "stm32h7xx_hal.h"
#elif defined(STM32H730xxQ)
	#include "stm32h7xx_hal.h"
#elif defined(STM32H733xx)
	#include "stm32h7xx_hal.h"
#elif defined(STM32H735xx)
	#include "stm32h7xx_hal.h"
#elif defined(STM32H742xx)
	#include "stm32h7xx_hal.h"
#elif defined(STM32H743xx)
	#include "stm32h7xx_hal.h"
#elif defined(STM32H745xG)
	#include "stm32h7xx_hal.h"
#elif defined(STM32H745xx)
	#include "stm32h7xx_hal.h"
#elif defined(STM32H747xG)
	#include "stm32h7xx_hal.h"
#elif defined(STM32H747xx)
	#include "stm32h7xx_hal.h"
#elif defined(STM32H750xx)
	#include "stm32h7xx_hal.h"
#elif defined(STM32H753xx)
	#include "stm32h7xx_hal.h"
#elif defined(STM32H755xx)
	#include "stm32h7xx_hal.h"
#elif defined(STM32H757xx)
	#include "stm32h7xx_hal.h"
#elif defined(STM32H7A3xx)
	#include "stm32h7xx_hal.h"
#elif defined(STM32H7A3xxQ)
	#include "stm32h7xx_hal.h"
#elif defined(STM32H7B0xx)
	#include "stm32h7xx_hal.h"
#elif defined(STM32H7B0xxQ)
	#include "stm32h7xx_hal.h"
#elif defined(STM32H7B3xx)
	#include "stm32h7xx_hal.h"
#elif defined(STM32H7B3xxQ)
	#include "stm32h7xx_hal.h"

#elif defined(STM32L010x4)
	#include "stm32l0xx_hal.h"
#elif defined(STM32L010x6)
	#include "stm32l0xx_hal.h"
#elif defined(STM32L010x8)
	#include "stm32l0xx_hal.h"
#elif defined(STM32L010xB)
	#include "stm32l0xx_hal.h"
#elif defined(STM32L011xx)
	#include "stm32l0xx_hal.h"
#elif defined(STM32L021xx)
	#include "stm32l0xx_hal.h"
#elif defined(STM32L031xx)
	#include "stm32l0xx_hal.h"
#elif defined(STM32L041xx)
	#include "stm32l0xx_hal.h"
#elif defined(STM32L051xx)
	#include "stm32l0xx_hal.h"
#elif defined(STM32L052xx)
	#include "stm32l0xx_hal.h"
#elif defined(STM32L053xx)
	#include "stm32l0xx_hal.h"
#elif defined(STM32L062xx)
	#include "stm32l0xx_hal.h"
#elif defined(STM32L063xx)
	#include "stm32l0xx_hal.h"
#elif defined(STM32L071xx)
	#include "stm32l0xx_hal.h"
#elif defined(STM32L072xx)
	#include "stm32l0xx_hal.h"
#elif defined(STM32L073xx)
	#include "stm32l0xx_hal.h"
#elif defined(STM32L081xx)
	#include "stm32l0xx_hal.h"
#elif defined(STM32L082xx)
	#include "stm32l0xx_hal.h"
#elif defined(STM32L083xx)
	#include "stm32l0xx_hal.h"

#elif defined(STM32L100xB)
	#include "stm32l1xx_hal.h"
#elif defined(STM32L100xBA)
	#include "stm32l1xx_hal.h"
#elif defined(STM32L100xC)
	#include "stm32l1xx_hal.h"
#elif defined(STM32L151xB)
	#include "stm32l1xx_hal.h"
#elif defined(STM32L151xBA)
	#include "stm32l1xx_hal.h"
#elif defined(STM32L151xC)
	#include "stm32l1xx_hal.h"
#elif defined(STM32L151xCA)
	#include "stm32l1xx_hal.h"
#elif defined(STM32L151xD)
	#include "stm32l1xx_hal.h"
#elif defined(STM32L151xDx)
	#include "stm32l1xx_hal.h"
#elif defined(STM32L151xE)
	#include "stm32l1xx_hal.h"
#elif defined(STM32L152xB)
	#include "stm32l1xx_hal.h"
#elif defined(STM32L152xBA)
	#include "stm32l1xx_hal.h"
#elif defined(STM32L152xC)
	#include "stm32l1xx_hal.h"
#elif defined(STM32L152xCA)
	#include "stm32l1xx_hal.h"
#elif defined(STM32L152xD)
	#include "stm32l1xx_hal.h"
#elif defined(STM32L152xDx)
	#include "stm32l1xx_hal.h"
#elif defined(STM32L152xE)
	#include "stm32l1xx_hal.h"
#elif defined(STM32L162xC)
	#include "stm32l1xx_hal.h"
#elif defined(STM32L162xCA)
	#include "stm32l1xx_hal.h"
#elif defined(STM32L162xD)
	#include "stm32l1xx_hal.h"
#elif defined(STM32L162xDx)
	#include "stm32l1xx_hal.h"
#elif defined(STM32L162xE)
	#include "stm32l1xx_hal.h"

#elif defined(STM32L412xx)
	#include "stm32l4xx_hal.h"
#elif defined(STM32L422xx)
	#include "stm32l4xx_hal.h"
#elif defined(STM32L431xx)
	#include "stm32l4xx_hal.h"
#elif defined(STM32L432xx)
	#include "stm32l4xx_hal.h"
#elif defined(STM32L433xx)
	#include "stm32l4xx_hal.h"
#elif defined(STM32L442xx)
	#include "stm32l4xx_hal.h"
#elif defined(STM32L443xx)
	#include "stm32l4xx_hal.h"
#elif defined(STM32L451xx)
	#include "stm32l4xx_hal.h"
#elif defined(STM32L452xx)
	#include "stm32l4xx_hal.h"
#elif defined(STM32L462xx)
	#include "stm32l4xx_hal.h"
#elif defined(STM32L471xx)
	#include "stm32l4xx_hal.h"
#elif defined(STM32L475xx)
	#include "stm32l4xx_hal.h"
#elif defined(STM32L476xx)
	#include "stm32l4xx_hal.h"
#elif defined(STM32L485xx)
	#include "stm32l4xx_hal.h"
#elif defined(STM32L486xx)
	#include "stm32l4xx_hal.h"
#elif defined(STM32L496xx)
	#include "stm32l4xx_hal.h"
#elif defined(STM32L4A6xx)
	#include "stm32l4xx_hal.h"
#elif defined(STM32L4P5xx)
	#include "stm32l4xx_hal.h"
#elif defined(STM32L4Q5xx)
	#include "stm32l4xx_hal.h"
#elif defined(STM32L4R5xx)
	#include "stm32l4xx_hal.h"
#elif defined(STM32L4R7xx)
	#include "stm32l4xx_hal.h"
#elif defined(STM32L4R9xx)
	#include "stm32l4xx_hal.h"
#elif defined(STM32L4S5xx)
	#include "stm32l4xx_hal.h"
#elif defined(STM32L4S7xx)
	#include "stm32l4xx_hal.h"
#elif defined(STM32L4S9xx)
	#include "stm32l4xx_hal.h"

#elif defined(STM32L552xx)
	#include "stm32l5xx_hal.h"
#elif defined(STM32L562xx)
	#include "stm32l5xx_hal.h"

#elif defined(STM32MP151Axx)
	#include "stm32mp1xx_hal.h"
#elif defined(STM32MP151Cxx)
	#include "stm32mp1xx_hal.h"
#elif defined(STM32MP153Axx)
	#include "stm32mp1xx_hal.h"
#elif defined(STM32MP153Cxx)
	#include "stm32mp1xx_hal.h"
#elif defined(STM32MP157Axx)
	#include "stm32mp1xx_hal.h"
#elif defined(STM32MP157Cxx)
	#include "stm32mp1xx_hal.h"
#elif defined(STM32MP15xx)
	#include "stm32mp1xx_hal.h"

#elif defined(STM32U031xx)
	#include "stm32u0xx_hal.h"
#elif defined(STM32U073xx)
	#include "stm32u0xx_hal.h"
#elif defined(STM32U083xx)
	#include "stm32u0xx_hal.h"

#elif defined(STM32U375xx)
	#include "stm32u3xx_hal.h"
#elif defined(STM32U385xx)
	#include "stm32u3xx_hal.h"

#elif defined(STM32U535xx)
	#include "stm32u5xx_hal.h"
#elif defined(STM32U545xx)
	#include "stm32u5xx_hal.h"
#elif defined(STM32U575xx)
	#include "stm32u5xx_hal.h"
#elif defined(STM32U585xx)
	#include "stm32u5xx_hal.h"
#elif defined(STM32U595xx)
	#include "stm32u5xx_hal.h"
#elif defined(STM32U599xx)
	#include "stm32u5xx_hal.h"
#elif defined(STM32U5A5xx)
	#include "stm32u5xx_hal.h"
#elif defined(STM32U5A9xx)
	#include "stm32u5xx_hal.h"
#elif defined(STM32U5F7xx)
	#include "stm32u5xx_hal.h"
#elif defined(STM32U5F9xx)
	#include "stm32u5xx_hal.h"
#elif defined(STM32U5G7xx)
	#include "stm32u5xx_hal.h"
#elif defined(STM32U5G9xx)
	#include "stm32u5xx_hal.h"

#elif defined(STM32WBA50xx)
	#include "stm32wbaxx_hal.h"
#elif defined(STM32WBA52xx)
	#include "stm32wbaxx_hal.h"
#elif defined(STM32WBA54xx)
	#include "stm32wbaxx_hal.h"
#elif defined(STM32WBA55xx)
	#include "stm32wbaxx_hal.h"
#elif defined(STM32WBA5Mxx)
	#include "stm32wbaxx_hal.h"

#elif defined(STM32WB10xx)
	#include "stm32wbxx_hal.h"
#elif defined(STM32WB15xx)
	#include "stm32wbxx_hal.h"
#elif defined(STM32WB1Mxx)
	#include "stm32wbxx_hal.h"
#elif defined(STM32WB30xx)
	#include "stm32wbxx_hal.h"
#elif defined(STM32WB35xx)
	#include "stm32wbxx_hal.h"
#elif defined(STM32WB50xx)
	#include "stm32wbxx_hal.h"
#elif defined(STM32WB55xx)
	#include "stm32wbxx_hal.h"
#elif defined(STM32WB5Mxx)
	#include "stm32wbxx_hal.h"

#elif defined(STM32WL54xx)
	#include "stm32wlxx_hal.h"
#elif defined(STM32WLE4xx)
	#include "stm32wlxx_hal.h"
#elif defined(STM32WLE5xx)
	#include "stm32wlxx_hal.h"
#else
	#error UNKNOWN CHIP
#endif
#endif /* INTERFACE_STM32_STM32XXYY_HAL_H_ */
