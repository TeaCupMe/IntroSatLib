# IntroSatLib host unit tests

DI MVP with a single public bus type `interfaces::I2C` (virtual API).
Platform backends: `ARDUINO`, `STM32_HAL`, and host **`VIRTUAL`**.

## Two I2C test layers

| Layer | Type | Use |
|-------|------|-----|
| DI `MockI2C` | `tests/fakes/MockI2C.hpp` | Inject into drivers; `bus.expect({...})` + `REQUIRE_I2C_CALLS(bus)` |
| VIRTUAL backend | `src/Adapter/I2C/VIRTUAL_I2CAdapter.cpp` + `CallLog` | Handle / pool / full-Core host path (`I2C_HANDLE` → `I2CAdapter::from`) |

`BusOp` / `BusCall` live in `tests/fakes/BusOp.h` and `BusCall.h`; `CallLog` is the host recording util those adapters write into.

See project-store docs `mock-i2c-expect-queue.md`, `test-harness-slimdown.md`,
`di-virtual-adapters-design.md`.

## Configure and run

```bash
cmake -S . -B build-host \
  -DINTROSATLIB_BUILD_TESTS=ON \
  -DINTROSATLIB_BACKEND=VIRTUAL \
  -DINTROSATLIB_ENABLE_INSTALL=OFF \
  -DCMAKE_CXX_COMPILER=g++
cmake --build build-host --target introsatlib_unit_tests
ctest --test-dir build-host --output-on-failure
```

`INTROSATLIB_BUILD_TESTS=ON` forces `INTROSATLIB_BACKEND=VIRTUAL`, creates the
`introsatlib_test_platform` INTERFACE target with STM32-shaped HAL stubs, and
links `VIRTUAL_I2CAdapter` (plus remaining host GPIO/SPI/UART/System stubs)
instead of `STM32_*.cpp` / `ARDUINO_*.cpp`. No Cube/Arduino SDK is required.
