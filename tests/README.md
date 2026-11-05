# IntroSatLib host unit tests

DI MVP with a single public bus type `interfaces::I2C` (virtual API).
Firmware backends: `ARDUINO` and `STM32_HAL`.

Driver unit tests inject **`MockI2C`** by reference (Catch2 v3): program the
scenario with `bus.expect({...})` (ops + `.respond(...)` / `.status(...)`), then
assert with `REQUIRE_I2C_CALLS(bus)`.

Host test builds **do not compile or link** `I2CAdapter` / pool / any I2C
adapter `.cpp`. Handle constructors are omitted under
`INTROSATLIB_HOST_UNIT_TESTS`. Non-I2C host stubs (GPIO/SPI/UART/System +
`CallLog`) remain so full-Core can link; `REQUIRE_SYSTEM_DELAY` uses FakeSystem.

See project-store docs `mock-i2c-expect-queue.md`, `test-harness-slimdown.md`,
`di-virtual-adapters-design.md`.

## Configure and run

```bash
cmake -S . -B build-host \
  -DINTROSATLIB_BUILD_TESTS=ON \
  -DINTROSATLIB_BACKEND=STM32_HAL \
  -DINTROSATLIB_ENABLE_INSTALL=OFF \
  -DCMAKE_CXX_COMPILER=g++
cmake --build build-host --target introsatlib_unit_tests
ctest --test-dir build-host --output-on-failure
```

`INTROSATLIB_BUILD_TESTS=ON` creates the `introsatlib_test_platform` INTERFACE
target with STM32-shaped HAL stubs. No Cube/Arduino SDK is required.
