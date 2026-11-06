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

### MockI2C call logging (optional)

Silent by default. Enable full per-call logs via **HostLog** (timestamp, level,
`[MockI2C]` tag, bus metadata, ANSI color on TTY):

```cpp
bus.setBusName("imu");
bus.setLogging(true);
```

or:

```bash
export INTROSATLIB_MOCK_I2C_LOG=1
# optional: INTROSATLIB_LOG_LEVEL=DEBUG|INFO|WARN|ERROR
# optional: NO_COLOR=1 or INTROSATLIB_LOG_COLOR=always
ctest --preset host-tests
```

Example: `14:02:11.042 DEBUG [MockI2C] bus=imu idx=1 op=ReadMem addr=0x6B reg=0x0F n=1 data=[0x69] -> OK`

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

