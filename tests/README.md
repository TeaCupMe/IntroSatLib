# IntroSatLib host unit tests

Phase 1 driver unit tests: real drivers + `I2CDevice`, recording fakes at the
`interfaces::{I2C,SPI,GPIO,UART}` / `system::*` Adapter seam (Catch2 v3).

See `docs/driver-unit-test-plan.md` in the project store for the approved plan.

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
