# IntroSatLib host unit tests

Driver I2C tests inject **`MockI2C`** (`interfaces::I2C`) with Catch2 v3:

```cpp
bus.expect({ IsReady(0x48), ReadMem(0x48, 0x00, 2).respond({0x19, 0x00}) });
// ... exercise driver ...
REQUIRE_I2C_CALLS(bus);
```

Host builds define `INTROSATLIB_HOST_UNIT_TESTS`:

- No `I2CAdapter*.cpp` / pool, no GPIO/SPI/UART fakes, no HAL stub
- `I2CAdapter.h` / `System.h` take a host preprocessor branch (opaque handle type)
- `tests/fakes/HostSystem.cpp` provides no-op `system::*` (e.g. `Delay` in InitGyro)
- Handle constructors are omitted in driver `.cpp` under this flag

Other bus interfaces (GPIO/SPI/UART) will follow the same Mock pattern later.

See project-store docs `mock-i2c-expect-queue.md`.

## Configure and run

```bash
cmake -S . -B build-host \
  -DINTROSATLIB_BUILD_TESTS=ON \
  -DINTROSATLIB_ENABLE_INSTALL=OFF \
  -DCMAKE_CXX_COMPILER=g++
cmake --build build-host --target introsatlib_unit_tests
ctest --test-dir build-host --output-on-failure
```
