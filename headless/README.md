# Headless IPC MVP (TCP + ISP-lite)

Host-only demo: virtual I2C over localhost TCP. No Cube/Arduino.

- **`introsatlib_sim_lm75a`** — fake LM75A register map (temp ≈ 25 °C)
- **`introsatlib_headless_demo`** — `TcpI2CBus` (`I2CBus`) + thin LM75A client (`readMem`)

Uses the real library seam `src/Adapter/I2CBus.h` (from the Driver test / DI MVP). `TcpI2CBus` implements that interface; demos stay under `headless/` only.

## Build

From repo root:

```bash
cmake -S . -B build-headless \
  -DINTROSATLIB_BUILD_HEADLESS=ON \
  -DINTROSATLIB_ENABLE_INSTALL=OFF
cmake --build build-headless --target introsatlib_sim_lm75a introsatlib_headless_demo
```

Or standalone:

```bash
cmake -S headless -B build-headless-standalone
cmake --build build-headless-standalone
```

## Run

```bash
# terminal 1
./build-headless/headless/introsatlib_sim_lm75a --port 17375

# terminal 2
./build-headless/headless/introsatlib_headless_demo --host 127.0.0.1 --port 17375
```

Expected client output:

```text
OK raw=0x1900 temp_c=25
```
