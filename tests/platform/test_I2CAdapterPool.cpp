#include <gtest/gtest.h>

#include "Device/I2CDevice.h"
#include "I2CScanner.h"

namespace {
TwoWire startupWire;
IntroSatLib::I2CDevice startupDevice(startupWire, 0x48);
}

TEST(I2CAdapterPool, SupportsDevicesConstructedBeforeMain)
{
	const unsigned before = startupWire.probes;
	ASSERT_EQ(startupDevice.Init(), ISL_OK);
	EXPECT_EQ(startupWire.probes, before + 1);
	EXPECT_EQ(startupWire.lastAddress, 0x48);
	EXPECT_EQ(&IntroSatLib::interfaces::I2CAdapter::from(startupWire),
		&IntroSatLib::interfaces::I2CAdapter::from(&startupWire));
}

TEST(I2CAdapterPool, ExhaustionPreservesPreviouslyIssuedBuses)
{
	using IntroSatLib::interfaces::I2C;
	using IntroSatLib::interfaces::I2CAdapter;
	static TwoWire wires[8];
	// Invalid handles must not consume any of the seven remaining slots.
	EXPECT_EQ(I2CAdapter::from(static_cast<TwoWire*>(nullptr)).isReady(0x48), ISL_ERROR);
	I2C* buses[7];
	for (unsigned i = 0; i < 7; ++i) {
		buses[i] = &I2CAdapter::from(wires[i]);
	}
	I2C& overflow = I2CAdapter::from(wires[7]);
	uint8_t data = 0;
	EXPECT_EQ(overflow.isReady(0x48), ISL_ERROR);
	EXPECT_EQ(overflow.read(0x48, &data, 1), ISL_ERROR);
	EXPECT_EQ(overflow.readMem(0x48, 0, &data, 1), ISL_ERROR);
	EXPECT_EQ(overflow.write(0x48, &data, 1), ISL_ERROR);
	EXPECT_EQ(overflow.writeMem(0x48, 0, &data, 1), ISL_ERROR);
	EXPECT_EQ(wires[7].probes, 0u);
	EXPECT_EQ(startupDevice.Init(), ISL_OK);
	for (unsigned i = 0; i < 7; ++i) {
		const unsigned before = wires[i].probes;
		EXPECT_EQ(buses[i]->isReady(0x30 + i), ISL_OK);
		EXPECT_EQ(wires[i].probes, before + 1);
		EXPECT_EQ(wires[i].lastAddress, 0x30 + i);
		EXPECT_EQ(&I2CAdapter::from(wires[i]), buses[i]);
	}
}

enum class ScannerInput { WireReference, HandlePointer, Interface };

class I2CScannerTest : public ::testing::TestWithParam<ScannerInput> {
protected:
	void TearDown() override { startupWire.result = 0; }
};

TEST_P(I2CScannerTest, AcceptsBusAndStopsRegisterScansOnNack)
{
	using IntroSatLib::I2CScanner;
	HardwareSerial serial;
	startupWire.result = 2; // NACK: register scans should stop at their probe.
	const unsigned before = startupWire.probes;
	switch (GetParam()) {
	case ScannerInput::WireReference:
		I2CScanner::scanI2CFull(startupWire, serial);
		I2CScanner::scanI2C(startupWire, serial);
		I2CScanner::scanDeviceRegisters(startupWire, 0x48, 0, 1, serial);
		I2CScanner::scanDeviceRegistersBatch(startupWire, 0x48, 0, 1, serial);
		break;
	case ScannerInput::HandlePointer:
		I2CScanner::scanI2CFull(&startupWire, serial);
		I2CScanner::scanI2C(&startupWire, serial);
		I2CScanner::scanDeviceRegisters(&startupWire, 0x48, 0, 1, serial);
		I2CScanner::scanDeviceRegistersBatch(&startupWire, 0x48, 0, 1, serial);
		break;
	case ScannerInput::Interface: {
		auto& bus = IntroSatLib::interfaces::I2CAdapter::from(startupWire);
		I2CScanner::scanI2CFull(bus, serial);
		I2CScanner::scanI2C(bus, serial);
		I2CScanner::scanDeviceRegisters(bus, 0x48, 0, 1, serial);
		I2CScanner::scanDeviceRegistersBatch(bus, 0x48, 0, 1, serial);
		break;
	}
	}
	EXPECT_EQ(startupWire.probes, before + 254);
	EXPECT_EQ(startupWire.lastAddress, 0x48);
	EXPECT_NE(serial.output.find("Device is not available!"), std::string::npos);
}

INSTANTIATE_TEST_SUITE_P(BusInputs, I2CScannerTest,
	::testing::Values(ScannerInput::WireReference, ScannerInput::HandlePointer, ScannerInput::Interface),
	[](const ::testing::TestParamInfo<ScannerInput>& info) {
		switch (info.param) {
		case ScannerInput::WireReference: return "WireReference";
		case ScannerInput::HandlePointer: return "HandlePointer";
		default: return "Interface";
		}
	});

// Keep the real pool definition after the global caller in this translation
// unit: a namespace-scope pool must not require later dynamic initialization.
#include "Adapter/I2C/I2CAdapterPool.cpp"
