# IntroSatLib host unit tests

DI MVP with a single public bus type `interfaces::I2C` (virtual API).
Platform backends for firmware: `ARDUINO` and `STM32_HAL`.

Driver unit tests inject **`MockI2C`** by reference (Catch2 v3): program the
scenario with `bus.expect({...})` (ops + `.respond(...)` / `.status(...)`), then
assert with `REQUIRE_I2C_CALLS(bus)`. There is no host I2C platform backend —
handle / `I2CAdapter::from` paths are not covered here.

`INTROSATLIB_BUILD_TESTS=ON` still links host stubs for GPIO/SPI/UART/System
(via `CallLog`) so full-Core can build, plus a minimal `I2CAdapterHostStub.cpp`
only to satisfy unused handle-ctor link symbols. `REQUIRE_SYSTEM_DELAY` uses
FakeSystem + CallLog.

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
