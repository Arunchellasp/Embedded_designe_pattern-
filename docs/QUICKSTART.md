# Quick Start Guide

## Prerequisites

### 1. Install ARM Toolchain

```bash
# Ubuntu/Debian
sudo apt-get install gcc-arm-none-eabi

# macOS
brew install arm-none-eabi-gcc

# Windows
# Download from: https://developer.arm.com/downloads/-/gnu-rm
```

### 2. Verify Installation

```bash
arm-none-eabi-gcc --version
# Expected: arm-none-eabi-gcc (GCC version) ...
```

---

## Building the Firmware

### Default Build (Debug Mode)

```bash
./build.sh

# Or using make directly
make
```

**Output:**
```
build/STM32F412ZET6/debug/output/
├── embedded_firmware.elf      (Executable)
├── embedded_firmware.bin      (Binary for flashing)
└── embedded_firmware.map      (Symbol map)
```

### Release Build (Optimized)

```bash
./build.sh MODE=release
```

### With Different HAL

```bash
./build.sh HAL=ll              # STM32 Low-Level
./build.sh HAL=opencm3        # libopencm3
```

### View Build Configuration

```bash
./build.sh info
```

### Clean Build

```bash
./build.sh clean               # Remove build artifacts
./build.sh distclean          # Full cleanup
```

---

## Project Structure Quick Reference

```
├── main.c              ← Entry point
├── app/                ← Your application logic here
├── drivers/            ← Use these in your app (GPIO, UART, etc.)
├── hal/                ← HAL abstraction (don't use directly)
├── bsp/                ← Board/clock config (don't modify often)
├── build.sh            ← Build script
└── Makefile            ← Professional build system
```

---

## Making Your First Change

### Example: Blink an LED

**Edit `app/app.c`:**

```c
error_t app_run(void)
{
    if (app_state != APP_STATE_RUNNING) {
        return ERR_NOT_INITIALIZED;
    }

    heartbeat_counter++;

    // Blink LED every 1000 iterations
    if (heartbeat_counter % 1000 == 0) {
        gpio_driver_toggle((gpio_pin_t)LED_PIN);  // ← LED pin defined in bsp/board_config.h
    }

    error_t err = app_health_check();
    if (err != ERR_OK) {
        app_state = APP_STATE_ERROR;
        return err;
    }

    return ERR_OK;
}
```

**Rebuild:**

```bash
./build.sh
```

---

## Flashing to Device

### With STM32CubeProgrammer

```bash
STM32_Programmer_CLI -c port=SWD freq=4M \
  -w build/STM32F412ZET6/debug/output/embedded_firmware.bin 0x08000000 \
  -v
```

### With OpenOCD

```bash
# Install OpenOCD
sudo apt-get install openocd

# Flash
openocd -f interface/stlink-v2.cfg \
        -f target/stm32f4x.cfg \
        -c "program build/STM32F412ZET6/debug/output/embedded_firmware.elf verify reset exit"
```

### With J-Link

```bash
JLinkExe -device STM32F412ZET6 -speed 4000 \
  -CommandFile flash.jlink
```

---

## Common Tasks

### Add a New Peripheral Driver

1. Create `drivers/new_driver.h` (interface)
2. Create `drivers/new_driver.c` (implementation using HAL)
3. Add to `Makefile` C_SOURCES
4. Use in `app/app.c`

### Change HAL Implementation

Simply rebuild with different HAL:

```bash
./build.sh HAL=opencm3         # Switch from STM32 HAL to libopencm3
```

No application code changes needed!

### Check for Compiler Warnings

```bash
./build.sh MODE=debug
# Build output will show all warnings
```

### Analyze Binary Size

```bash
make size
# Shows: text, data, bss sizes
```

---

## Build System Options

```bash
# Syntax: ./build.sh [OPTION=value]

BOARD=STM32F412ZET6             # Target MCU
HAL=stm32_hal                   # HAL implementation
MODE=debug                      # Build mode (debug/release)
JOBS=4                          # Parallel build jobs

# Examples:
./build.sh BOARD=STM32F407ZGT6 HAL=ll MODE=release JOBS=8
```

---

## Troubleshooting

### "arm-none-eabi-gcc not found"

```bash
# Check installation
which arm-none-eabi-gcc

# Install if missing
sudo apt-get install gcc-arm-none-eabi
```

### Build errors with warnings

The framework treats warnings as errors (`-Werror`). Fix warnings:

```bash
# See all warnings
make clean && make

# Fix issues in source code
# Then rebuild
./build.sh
```

### "Permission denied: ./build.sh"

```bash
chmod +x build.sh
./build.sh
```

---

## Next Steps

1. **Read Architecture Documentation:** `docs/ARCHITECTURE.md`
2. **Learn Error Handling:** See `common/error.h`
3. **Understand HAL Abstraction:** See `hal/hal_gpio.h`
4. **Write Application Logic:** Edit `app/app.c`
5. **Add New Drivers:** Follow driver pattern in `drivers/`

---

## Getting Help

### Check Documentation

- `docs/ARCHITECTURE.md` - Full architecture guide
- `docs/HOW_TO_ADD_MCU.md` - Adding new MCUs
- `Makefile` - Build system documentation
- `build.sh help` - Build script help

### Common Questions

**Q: How do I change the LED pin?**  
A: Edit `bsp/board_config.h` - Find `LED_PIN` definition

**Q: How do I use UART?**  
A: Use `uart_driver_write()` and `uart_driver_read()` from `drivers/uart_driver.h`

**Q: How do I add a new MCU?**  
A: See `docs/ARCHITECTURE.md` - Section "How to Add a New MCU"

---

**Start coding! 🚀**
