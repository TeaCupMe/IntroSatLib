# IntroSatLib host unit tests

DI MVP with a single public bus type `interfaces::I2C` (virtual API).
Platform backends use library-internal `interfaces::I2CAdapter`.
Tests inject `FakeI2C` by reference (Catch2 v3) and assert sequences with
`REQUIRE_I2C_CALLS` / `REQUIRE_SYSTEM_DELAY` from `I2CCallHelpers.hpp`.
Host stubs still compile production Adapter headers; link fakes remain for
`system::*` (e.g. Delay) and full-Core host builds.

See project-store docs `test-harness-slimdown.md`, `di-virtual-adapters-design.md`.

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
target with STM32-shaped HAL stubs and links Adapter fakes instead of
`Adapter/STM32/*.cpp`. No Cube/Arduino SDK is required.
