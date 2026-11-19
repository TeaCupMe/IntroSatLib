# IntroSatLib host unit tests

Host tests use **GoogleTest + Google Mock v1.17.0** (downloaded by CMake).
`MockI2C.hpp` declares the five methods of `interfaces::I2C` with `MOCK_METHOD`.
Use `StrictMock<MockI2C>` to reject bus operations without matching expectations:

```cpp
using namespace ::testing;
StrictMock<IntroSatLib::test::MockI2C> bus;
const uint8_t response[] = {0x19, 0x00};
EXPECT_CALL(bus, readMem(0x48, 0x00, _, 2))
    .WillOnce(DoAll(SetArrayArgument<2>(response, response + 2), Return(ISL_OK)));
IntroSatLib::LM75A sensor(bus);
EXPECT_NEAR(sensor.GetTemperature(), 25.0f, 0.001f);
```

Expectations are verified automatically when the mock is destroyed. For an exact
transaction sequence, create an `InSequence` object before setting expectations.
Use `Pointee` for single-byte write payloads, `SetArgPointee` / `SetArrayArgument`
for read responses, and `Return(ISL_ERROR)` / `Return(ISL_TIMEOUT)` for failures.
Response arrays passed to `SetArrayArgument` must stay alive until the call.

`INTROSATLIB_BUILD_TESTS=ON` selects host mode (`INTROSATLIB_HOST_UNIT_TESTS` in
headers): driver tests do not link the platform I2C adapter or HAL. `HostSystem.cpp`
provides no-op `system::*`. Firmware `BACKEND` / `PLATFORM_TARGET` are unused.
GoogleTest and gMock are test-only dependencies and are not linked into firmware.

The driver tests also link the I2C-only drivers and legacy facades through
`MockI2C` to catch accidental dependencies on platform constructors. Drivers
that also require GPIO (IRCamera, M24C02) need GPIO test support separately.

The same build runs a separate `introsatlib_i2c_platform_tests` executable using
the real Arduino I2C adapter and small Wire/Serial doubles in `platform/arduino`.
It checks global device construction, pool exhaustion and scanner compatibility
with Wire references, handle pointers and the abstract I2C interface.

The firmware adapter pool retains up to eight distinct handle addresses for the
program lifetime. Handles must outlive their devices. A null handle or an
exhausted pool returns an unavailable adapter: all bus operations report
`ISL_ERROR`; previously created devices keep their original bus.

## Mock call diagnostics

The previous `bus.expect(...)`, `REQUIRE_I2C_CALLS`, and
`INTROSATLIB_MOCK_I2C_LOG` interface has been replaced by gMock expectations and
built-in call diagnostics. To print expectation setup and matched calls, run:

```powershell
.\build\host-tests\tests\introsatlib_unit_tests.exe --gmock_verbose=info --gtest_filter=LSM6DS3.*
```

Use `ctest --preset host-tests-mingw -V` to show output from passing tests.
Driver `LOGGER` output still uses the existing HostLog sink; its level and color
settings (`INTROSATLIB_LOG_LEVEL`, `NO_COLOR`, `INTROSATLIB_LOG_COLOR`) are retained.

## Configure and run (preset)

```bash
cmake --preset host-tests
cmake --build --preset host-tests
ctest --preset host-tests
```

On Windows with MSYS2 MinGW, prefer the mingw preset (it prepends
`C:/msys64/mingw64/bin` so other toolchains cannot shadow GCC DLLs):

```bash
cmake --preset host-tests-mingw
cmake --build --preset host-tests-mingw
ctest --preset host-tests-mingw
```

Equivalent without presets (Ninja):

```bash
cmake -S . -B build/host-tests -G Ninja \
  -DINTROSATLIB_BUILD_TESTS=ON \
  -DINTROSATLIB_ENABLE_INSTALL=OFF
cmake --build build/host-tests --target introsatlib_unit_tests
ctest --test-dir build/host-tests --output-on-failure
```

### Windows PATH note

If configure fails at `CMakeTestCXXCompiler` with `g++ ... Error 1` and **no
compiler stderr**, PATH almost certainly loads an incompatible
`libgcc_s_seh-1.dll` / `libstdc++-6.dll` before MSYS2’s (common culprit:
`STM32CubeProgrammer\bin`). `g++ -c` can still “work” in a cleaned shell.

Fix: put `C:\msys64\mingw64\bin` first on `PATH` for that session, or use
`host-tests-mingw`. Also avoid bare `-DCMAKE_CXX_COMPILER=g++` when multiple
`g++.exe` exist (`C:\MinGW`, MSYS2, etc.); prefer the full path to
`mingw64\bin\g++.exe`. Prefer `mingw32-make` / Ninja over GnuWin32 `make.exe`.

