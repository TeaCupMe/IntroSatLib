#ifndef I2CDEVICE_H_
#define I2CDEVICE_H_

#include "../Adapter/I2C.h"
#include "Device.h"

#include <new>
#include <type_traits>
#include <utility>

namespace IntroSatLib {

class I2CDevice: public Device 	{

private:
	/**
	 * Active bus (valid while this object is alive).
	 * Points at owned storage or an external I2CBus (DI / FakeI2CBus).
	 */
	interfaces::I2CBus* _bus = nullptr;

	/** Optional owned concrete Adapter created from Wire / HAL handle. */
	bool _hasOwned = false;
	alignas(interfaces::I2C) unsigned char _ownedStorage[sizeof(interfaces::I2C)];

	interfaces::I2C* ownedPtr()
	{
		return _hasOwned ? reinterpret_cast<interfaces::I2C*>(_ownedStorage) : nullptr;
	}

	const interfaces::I2C* ownedPtr() const
	{
		return _hasOwned ? reinterpret_cast<const interfaces::I2C*>(_ownedStorage) : nullptr;
	}

	void destroyOwned()
	{
		if (_hasOwned) {
			ownedPtr()->~I2C();
			_hasOwned = false;
		}
	}

	void emplaceOwned(interfaces::I2C adapter)
	{
		destroyOwned();
		new (_ownedStorage) interfaces::I2C(std::move(adapter));
		_hasOwned = true;
		_bus = ownedPtr();
	}

protected:

	uint8_t _address;

	interfaces::I2CBus& bus() { return *_bus; }
	const interfaces::I2CBus& bus() const { return *_bus; }

	ISL_StatusTypeDef IsReady();

	/**
	 * @brief checks if register contents equal to @b value
	 *
	 * @returns @b true, if register was read successfully and its contents match @value
	 * @returns @b false, if any error happened during communication or register contents do not match @b value
	 */
	bool CheckRegisterI2C(uint8_t reg, uint8_t value);

	ISL_StatusTypeDef WriteI2C(uint8_t byte);

	ISL_StatusTypeDef WriteI2C(uint8_t* buf, uint8_t nByte);

	ISL_StatusTypeDef ReadI2C(uint8_t* buf, uint8_t nBytes = 1);

	// TODO я почему-то разнёс на два метода, уже не помню почему...
	ISL_StatusTypeDef ReadRegisterI2C(uint8_t reg, uint8_t* data);

	ISL_StatusTypeDef ReadRegisterI2C(uint8_t reg, uint8_t* data, uint8_t nBytes);

	/**
	 * @brief Возвращает значение из регистра устройства
	 *
	 * @param reg Адрес регистра
	 * @return Значение решистра
	 */
	// TODO сделать через optional<uint8_t, ISL_StatusTypeDef>. Для этого нужен c++17, в кубе вроде дефолтно c++14.
	//															Переключится можно, но не хочется чтобы это было необходимо для каждого проекта...
	uint8_t GetRegisterI2C(uint8_t reg) __attribute__((deprecated("This method does not check if I2C read happened correctly")));

	ISL_StatusTypeDef SetRegisterI2C(uint8_t reg, uint8_t* value, uint8_t nBytes);

	/**
	 * @brief Запись значения в регистр устройства
	 *
	 * @param reg Адрес регистра
	 * @param value Значение
	 */
	ISL_StatusTypeDef SetRegisterI2C(uint8_t reg, uint8_t value);

	/**
	 * @brief Установка одного бита регистра в значение 1
	 *
	 * @param reg Адрес регистра
	 * @param bit Номер бита, где 0 - младший бит, 7 - старший бит.
	 */
	ISL_StatusTypeDef SetBitRegisterI2C(uint8_t reg, uint8_t bit);

	/**
	 * @brief Установка одного бита регистра в значение 0
	 *
	 * @param reg Адрес регистра
	 * @param bit Номер бита, где 0 - младший бит, 7 - старший бит.
	 */
	ISL_StatusTypeDef ResetBitRegisterI2C(uint8_t reg, uint8_t bit);

	/**
	 * @brief Установка одного бита регистра в значение по условию @ref value
	 *
	 * @param reg Адрес регистра
	 * @param bit Номер бита, где 0 - младший бит, 7 - старший бит.
	 * @param value Условие установки. Если true (&ne; 0) - выставляет в 1, иначе - в 0.
	 */
	ISL_StatusTypeDef BitRegisterI2C(uint8_t reg, uint8_t bit, uint8_t value);

public:
	/**
	 * @brief DI / tests: non-owning reference to any I2CBus except concrete I2C
	 * (avoids overload ambiguity with the owning I2C by-value ctor).
	 */
	template <
		typename Bus,
		typename std::enable_if<
			std::is_base_of<interfaces::I2CBus, Bus>::value
				&& !std::is_same<Bus, interfaces::I2C>::value,
			int>::type = 0>
	I2CDevice(Bus& bus, uint8_t address) : _bus(&bus), _address(address) {}

	/**
	 * @brief Owning concrete Adapter (Wire/hi2c converting ctor still works via temporary I2C).
	 */
	I2CDevice(interfaces::I2C i2c, uint8_t address);

	/**
	 * @brief Production convenience: wrap HAL / Wire handle inside an owned Adapter.
	 */
	I2CDevice(interfaces::I2C_HANDLE_TYPE& hi2c, uint8_t address);
	I2CDevice(interfaces::I2C_HANDLE_TYPE* hi2c, uint8_t address);

	I2CDevice(const I2CDevice& other);
	I2CDevice(I2CDevice&& other);
	I2CDevice& operator=(const I2CDevice& other);
	I2CDevice& operator=(I2CDevice&& other);

	/**
	 * @brief Инициализация устройства на шине I2C
	 *
	 * @returns 0, если инициализация завершена успешно
	 * @returns код ошибки HAL_StatusTypeDef, если при инициализации возникла ошибка
	 */
	virtual ISL_StatusTypeDef Init() override;
	virtual ~I2CDevice();
};

} /* namespace IntroSatLib */

#endif /* I2CDEVICE_H_ */
