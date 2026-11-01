# IntroSatLib host unit tests

DI MVP: real drivers + `I2CDevice` with abstract `interfaces::I2CBus`,
`FakeI2CBus` injected by reference (Catch2 v3). Host stubs still compile
production Adapter headers; link fakes remain for `system::*` (e.g. Delay).

See project-store docs `di-virtual-adapters-design.md` and `driver-unit-test-plan.md`.

## Configure and run

```bash
cmake -S . -B build-host \
  -DINTROSATLIB_BUILD_TESTS=ON \
  -DINTROSATLIB_BACKEND=STM32_HAL \
  -DINTROSATLIB_ENABLE_INSTALL=OFF
cmake --build build-host --target introsatlib_unit_tests
ctest --test-dir build-host --output-on-failure
```

Or run the binary directly:

```bash
./build-host/tests/introsatlib_unit_tests
```

`INTROSATLIB_BUILD_TESTS=ON` creates the `introsatlib_test_platform` INTERFACE
target with STM32-shaped HAL stubs and links Adapter fakes instead of
`Adapter/STM32/*.cpp`. No Cube/Arduino SDK is required.
