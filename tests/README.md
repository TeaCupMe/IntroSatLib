# IntroSatLib host unit tests

Driver I2C tests inject **`MockI2C`** (`interfaces::I2C`) with Catch2 v3:

```cpp
bus.expect({ IsReady(0x48), ReadMem(0x48, 0x00, 2).respond({0x19, 0x00}) });
// ... exercise driver ...
REQUIRE_I2C_CALLS(bus);
```

`INTROSATLIB_BUILD_TESTS=ON` selects host mode (`INTROSATLIB_HOST_UNIT_TESTS` in
headers): no `I2CAdapter*.cpp` / pool, no HAL stub; `HostSystem.cpp` provides
no-op `system::*`. Firmware `BACKEND` / `PLATFORM_TARGET` are unused.

## Configure and run (preset)

```bash
cmake --preset host-tests
cmake --build --preset host-tests
ctest --preset host-tests
```

Equivalent without presets:

```bash
cmake -S . -B build/host-tests \
  -DINTROSATLIB_BUILD_TESTS=ON \
  -DINTROSATLIB_ENABLE_INSTALL=OFF \
  -DCMAKE_CXX_COMPILER=g++
cmake --build build/host-tests --target introsatlib_unit_tests
ctest --test-dir build/host-tests --output-on-failure
```

