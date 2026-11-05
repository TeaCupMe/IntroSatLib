#ifndef TESTS_MOCKS_I2C_I2CDEVICEDESCRIPTOR_H_
#define TESTS_MOCKS_I2C_I2CDEVICEDESCRIPTOR_H_

#include <cstdint>
#include <array>

namespace IntroSatLib {
namespace mocks {

class I2CDeviceDescriptor {
private:
    uint8_t address_;
    std::vector<Register> registers;
    bool asyncUpdate = false;
public:
    struct Register {
        uint8_t address;
        uint8_t value;
        const uint8_t defaultValue;
        uint8_t (*updateValue)() = nullptr;

        void setValue(uint8_t value) 
        {
            this->value = value;
        }
    };

	I2CDeviceDescriptor(uint8_t address, std::vector<Register> regs) : address_(address), registers(regs) {}

	uint8_t getAddress() const { return address_; }

    bool getRegisterValue(const uint8_t address, uint8_t* output)
    {
        for (Register& reg: this->registers)
        {
            if (reg.address == address)
            {
                if (asyncUpdate && (reg.updateValue != nullptr))
                {
                    reg.updateValue();
                }

                *output = reg.value;
                return true;
            }
        }

        return false;
    }

    void reset()
    {
        for (Register& reg: this->registers)
        {
            reg.value = reg.defaultValue;
        }
    }
};

} // namespace mocks
} // namespace IntroSatLib

#endif // TESTS_MOCKS_I2C_I2CDEVICEDESCRIPTOR_H_