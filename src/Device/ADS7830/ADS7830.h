#ifndef ADS7830_H_
#define ADS7830_H_

#include "Device/I2CDevice.h"
#include "Device/Base/BaseADC.h"

#include "Adapter/System.h"
#include "Adapter/I2C.h"

#include "stdint.h"

namespace IntroSatLib {

class ADS7830: public BaseADC<16>, protected I2CDevice {
public:
    /**
     * @brief Канал измерения
     */
    enum Channel: uint8_t {
        DIFF_1_TO_0 = 0b0000, /**< Разность напряжений CH1-CH0 */
        DIFF_0_TO_1 = 0b0100, /**< Дифференциальное измерение, CH0-CH1 */
        DIFF_3_TO_2 = 0b0001, /**< Дифференциальное измерение, CH3-CH2 */
        DIFF_2_TO_3 = 0b0101, /**< Дифференциальное измерение, CH2-CH3 */
        DIFF_5_TO_4 = 0b0010, /**< Дифференциальное измерение, CH5-CH4 */
        DIFF_4_TO_5 = 0b0110, /**< Дифференциальное измерение, CH4-CH5 */
        DIFF_7_TO_6 = 0b0011, /**< Дифференциальное измерение, CH7-CH6 */
        DIFF_6_TO_7 = 0b0111, /**< Дифференциальное измерение, CH6-CH7 */
        CH0         = 0b1000, /**< Напряжение на канале CH0 */
        CH1         = 0b1100, /**< Напряжение на канале CH1 */
        CH2         = 0b1001, /**< Напряжение на канале CH2 */
        CH3         = 0b1101, /**< Напряжение на канале CH3 */
        CH4         = 0b1010, /**< Напряжение на канале CH4 */
        CH5         = 0b1110, /**< Напряжение на канале CH5 */
        CH6         = 0b1011, /**< Напряжение на канале CH6 */
        CH7         = 0b1111  /**< Напряжение на канале CH7 */
    };

    /**
     * @brief Опорное напряжение
     */

    enum ReferenceMode: uint8_t {
        REF_EXTERNAL = 0b0, /**< Внешнее опорного напряжение (пин REF<sub>IN</sub>) */
        REF_INTERNAL = 0b1  /**< Внутреннее опорного напряжение (+2.5В) */
    };

    /**
     * @brief Состояние АЦП
     */
    enum AdcOn : uint8_t {
        ADC_OFF = 0, /**< АЦП включён */
        ADC_ON  = 1  /**< АЦП выключен (режим power-down) */
    };

private:
    static const uint8_t BASE_ADDRESS = 0x48;
    const uint8_t channelCount = 16;

    uint8_t resolution = 8;
    float _coeffConvert[]; // to convert to volts with bit-depth 8
    ReferenceMode refMode


   ISL_StatusTypeDef SendCommand(Mode mode, Channel channel, ReferenceMode ref, AdcOn on);

public:
    ADS7830(interfaces::I2C i2c, uint8_t address = BASE_ADDRESS);
    
    ISL_StatusTypeDef Init() override;
    ISL_StatusTypeDef Init(float uRef);
    ISL_StatusTypeDef InitChannel(Channel channel, float coefficient);

    void SetAdcOn(AdcOn on);

    ISL_StatusTypeDef SetRef(float refVoltage = 2.5);
    ISL_StatusTypeDef PowerDown();
    
    uint8_t GetRawValue(Channel channel);
    
    float GetValue(Channel channel); 
    float GetValue(float coeff, Channel channel); 
};

}

#endif