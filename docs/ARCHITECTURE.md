# Industrial-Grade Embedded Firmware Framework

## Overview

This is a production-ready embedded firmware framework for STM32 microcontrollers, designed with:

- **Clean Layered Architecture** - Strict separation of concerns
- **Hardware Abstraction** - HAL-agnostic code
- **Multi-Board Support** - Easy MCU/board switching
- **Industrial Standards** - MISRA-inspired C practices
- **Professional Build System** - Reproducible builds with configuration flexibility
- **Error Handling** - Centralized, no silent failures
- **Portability** - Works with STM32 HAL, LL, libopencm3, and custom HALs

---

## Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                   APPLICATION LAYER (/app)                  │
│   - State machines, business logic, high-level control      │
└──────────────────┬──────────────────────────────────────────┘
                   │
┌──────────────────┴──────────────────────────────────────────┐
│                   SERVICES LAYER (/services)                │
│   - Logging, error management, scheduling, timers           │
└──────────────────┬──────────────────────────────────────────┘
                   │
┌──────────────────┴──────────────────────────────────────────┐
│                   DRIVERS LAYER (/drivers)                  │
│   - GPIO, UART, SPI, I2C drivers (independent of HAL)      │
└──────────────────┬──────────────────────────────────────────┘
                   │
┌──────────────────┴──────────────────────────────────────────┐
│             HAL ABSTRACTION LAYER (/hal)                    │
│   - Function-pointer based interfaces to select HAL at      │
│     compile time (STM32 HAL, LL, libopencm3, custom)       │
└──────────────────┬──────────────────────────────────────────┘
                   │
┌──────────────────┴──────────────────────────────────────────┐
│        BOARD SUPPORT PACKAGE (/bsp)                         │
│   - Clock config, pin mapping, peripheral enabling          │
├──────────────────┬──────────────────────────────────────────┤
│     PLATFORM (/platform)                                    │
│   - MCU startup, linker scripts, vector tables              │
└─────────────────────────────────────────────────────────────┘
```

### Dependency Rules (STRICT)

```
✓ Allowed:
  - Application → Services
  - Application → Drivers
  - Drivers → HAL
  - HAL → BSP
  - BSP → Platform
  - Any layer → /common (error, utilities)

✗ NOT Allowed:
  - Application → HAL (must use drivers)
  - Application → BSP
  - Drivers → Application
  - HAL → Application
```

---

## Folder Structure

```
embedded_firmware/
├── main.c                          # Entry point
├── Makefile                        # Professional build system
├── build.sh                        # Build script with validation
│
├── app/                            # Application layer
│   ├── app.h                       # Application interface
│   └── app.c                       # Application implementation
│
├── services/                       # Services layer (future expansion)
│   └── (logging, scheduler, etc.)
│
├── drivers/                        # Driver layer
│   ├── gpio_driver.h/.c            # GPIO driver
│   └── uart_driver.h/.c            # UART driver
│
├── hal/                            # HAL abstraction layer
│   ├── hal_gpio.h/.c               # GPIO HAL interface
│   ├── hal_uart.h/.c               # UART HAL interface
│   └── (other HAL interfaces)
│
├── bsp/                            # Board Support Package
│   ├── board_config.h              # Board configuration
│   ├── bsp_init.h/.c               # BSP initialization
│   └── bsp_clock.h/.c              # Clock configuration
│
├── platform/                       # Platform-specific code
│   ├── platform_startup.h/.c       # Startup code
│   └── linker.ld                   # Linker script (MCU-specific)
│
├── boards/                         # Board-specific files
│   └── STM32F412ZET6/              # Example board
│       └── board_specifics.h
│
├── common/                         # Shared utilities
│   ├── error.h/.c                  # Error handling
│   └── (macros, types, etc.)
│
├── build/                          # Build artifacts
│   └── STM32F412ZET6/
│       ├── debug/
│       │   ├── obj/                # Object files
│       │   └── output/             # ELF, BIN, MAP
│       └── release/
│
└── docs/                           # Documentation
    ├── ARCHITECTURE.md             # This file
    ├── QUICKSTART.md
    └── HOW_TO_ADD_MCU.md
```

---

## Key Features

### 1. Clean Layered Architecture

- **No circular dependencies**
- **Clear ownership and responsibility**
- **Easy to test** (mock layers below)
- **Portable** (swap HAL without touching app)

### 2. HAL Abstraction

Function-pointer based design allows runtime or compile-time HAL selection:

```c
// hal/hal_gpio.h
typedef struct {
    void (*init)(gpio_pin_t pin, gpio_mode_t mode, ...);
    void (*write)(gpio_pin_t pin, bool value);
    bool (*read)(gpio_pin_t pin);
    void (*toggle)(gpio_pin_t pin);
} gpio_hal_t;
```

**Compile-time selection:**
```bash
make HAL=stm32_hal
make HAL=ll
make HAL=opencm3
```

### 3. Error Handling

Industrial-grade error management:

```c
typedef enum {
    ERR_OK, ERR_TIMEOUT, ERR_HW_FAILURE, ERR_INVALID_PARAM, ...
} error_t;

typedef enum {
    SEVERITY_INFO, SEVERITY_WARN, SEVERITY_ERROR, SEVERITY_FATAL
} error_severity_t;

// Centralized error logging (no silent failures)
error_log(error_code, severity, context);
```

### 4. Professional Build System

```bash
# Simple build
./build.sh

# With options
./build.sh BOARD=STM32F412ZET6 HAL=stm32_hal MODE=release

# Clean
./build.sh clean

# Show config
./build.sh info
```

---

## How to Add a New MCU

### Step 1: Create Board Directory

```bash
mkdir -p boards/STM32F407ZGT6
```

### Step 2: Create Board Configuration

**boards/STM32F407ZGT6/board_config.h:**

```c
#define BOARD_STM32F407ZGT6
#define SYSTEM_CLOCK_HZ         168000000UL
#define AHB_CLOCK_HZ            168000000UL
#define APB1_CLOCK_HZ           84000000UL
#define APB2_CLOCK_HZ           168000000UL
#define FLASH_SIZE              0x100000    /* 1 MB */
#define RAM_SIZE                0x20000     /* 128 KB */

/* Pin mappings specific to this board */
#define LED_PORT                GPIOB
#define LED_PIN                 5
```

### Step 3: Add MCU-Specific BSP

**boards/STM32F407ZGT6/bsp_clock.c:**

```c
error_t bsp_clock_init(void)
{
    /* STM32F407 specific clock configuration:
     * - HSE: 8 MHz crystal
     * - PLL: 168 MHz (HSE × 42 / 8)
     * - AHB prescaler: 1
     * - APB1 prescaler: 4
     * - APB2 prescaler: 2
     */
    // TODO: Implement clock setup
    return ERR_OK;
}
```

### Step 4: Build with New MCU

```bash
make BOARD=STM32F407ZGT6
```

### Step 5: Add HAL Abstraction (if needed)

If using a new HAL library:

**hal/hal_gpio_opencm3.c:**

```c
static const gpio_hal_t opencm3_gpio_hal = {
    .init = opencm3_gpio_init,
    .write = opencm3_gpio_write,
    .read = opencm3_gpio_read,
    .toggle = opencm3_gpio_toggle
};
```

Then build with:

```bash
make BOARD=STM32F407ZGT6 HAL=opencm3
```

---

## How to Switch HAL Implementation

### Current Support

The framework is designed to support:

1. **STM32 HAL** (STM32Cube) - Default
2. **STM32 LL** (Low-Level) - For minimal footprint
3. **libopencm3** - Open-source alternative
4. **Custom HAL** - Your own implementation

### Switching HAL

```bash
# STM32 HAL (STM32Cube)
./build.sh HAL=stm32_hal

# STM32 LL
./build.sh HAL=ll

# libopencm3
./build.sh HAL=opencm3
```

### Adding a New HAL

1. Create HAL adapter in `hal/hal_gpio_<name>.c`
2. Implement the `gpio_hal_t` function pointers
3. Register in `hal_gpio_init()` with preprocessor check
4. Build: `make HAL=<name>`

---

## Build System Usage

### Basic Build

```bash
./build.sh                          # Default: debug mode, STM32F412ZET6
```

### Production Build

```bash
./build.sh MODE=release             # Optimized, no debug symbols
```

### Cross-Board Build

```bash
./build.sh BOARD=STM32F407ZGT6     # Different MCU
```

### Full Configuration

```bash
./build.sh BOARD=STM32F407ZGT6 HAL=opencm3 MODE=release
```

### Makefile Direct Usage

```bash
make all                            # Build
make clean                          # Clean build artifacts
make info                           # Show configuration
make size                           # Show binary size
```

---

## Coding Standards

This framework enforces:

- **MISRA-inspired C** (even without full MISRA)
- **No magic numbers** - All constants defined
- **No global variables** without justification
- **Explicit initialization** - All variables initialized
- **Clear ownership** - Who allocates, who frees
- **No silent failures** - All errors logged
- **Static functions** - Internal functions marked `static`
- **Clear naming** - Module prefix for public symbols

### Example Good Code

```c
// ✓ GOOD
typedef enum {
    STATE_IDLE = 0,
    STATE_ACTIVE
} led_state_t;

static led_state_t current_state = STATE_IDLE;
static const uint32_t BLINK_INTERVAL_MS = 500;

error_t led_init(gpio_pin_t pin)
{
    if (pin == 0) {
        return ERR_INVALID_PARAM;
    }
    return gpio_driver_configure(pin, GPIO_MODE_OUTPUT);
}
```

### Example Bad Code

```c
// ✗ BAD
led_state = 0;              // Magic number
gpio_pin = 10;              // Global without justification
if (init() < 0) ;           // Silent failure
#define X 500               // Unclear name and no module prefix
```

---

## Error Handling Pattern

```c
// Layer 1: Platform/HAL
error_t bsp_clock_init(void)
{
    /* Setup PLL */
    if (pll_setup_failed()) {
        error_log(ERR_HW_FAILURE, SEVERITY_FATAL, __LINE__);
        return ERR_HW_FAILURE;
    }
    return ERR_OK;
}

// Layer 2: Driver
error_t gpio_driver_init(void)
{
    error_t err = gpio_hal_init();
    if (err != ERR_OK) {
        error_log(err, SEVERITY_ERROR, __LINE__);
        return err;
    }
    return ERR_OK;
}

// Layer 3: Application
error_t app_init(void)
{
    error_t err = bsp_init();
    if (err != ERR_OK) {
        error_log(err, SEVERITY_FATAL, 0);
        return err;
    }
    return ERR_OK;
}
```

---

## Building & Flashing

### Generate Binary

```bash
./build.sh MODE=release

# Output files:
# - build/STM32F412ZET6/release/output/embedded_firmware.elf
# - build/STM32F412ZET6/release/output/embedded_firmware.bin
# - build/STM32F412ZET6/release/output/embedded_firmware.map
```

### Flash with STM32CubeProgrammer

```bash
STM32_Programmer_CLI -c port=SWD freq=4M mode=UR \
  -w build/STM32F412ZET6/release/output/embedded_firmware.bin 0x08000000 \
  -v
```

### Flash with OpenOCD

```bash
openocd -f interface/stlink-v2.cfg \
        -f target/stm32f4x.cfg \
        -c "program build/STM32F412ZET6/release/output/embedded_firmware.elf verify reset exit"
```

---

## Testing & Debugging

### Compile-Time Validation

```bash
# Enable all warnings
make CFLAGS="-Wall -Wextra -Werror"

# Strict mode
make STRICT=1
```

### Size Analysis

```bash
./build.sh info          # Shows configuration
make size                # Shows ELF size breakdown
arm-none-eabi-objdump -h build/.../embedded_firmware.elf
```

### Debug Info

```bash
./build.sh MODE=debug    # Includes debug symbols (-g3)
arm-none-eabi-gdb build/STM32F412ZET6/debug/output/embedded_firmware.elf
```

---

## Best Practices

### 1. Always Check Return Values

```c
error_t err = uart_driver_write(UART_1, data, length);
if (err != ERR_OK) {
    return err;
}
```

### 2. Initialize BSP Before Hardware

```c
bsp_init();      // Clock, GPIO clocks
gpio_driver_init();
uart_driver_init();
```

### 3. Use Constants, Not Magic Numbers

```c
#define LED_PIN 5
#define UART_BAUD 115200
```

### 4. Minimize Global State

```c
// ✓ Good: Encapsulated state
typedef struct {
    uart_id_t uart_id;
    uint32_t baud;
} uart_handle_t;

// ✗ Bad: Global state
static uint32_t g_uart_baud = 115200;
```

### 5. Document Your Layers

```c
/*
 * uart_driver.h - UART Driver
 *
 * Application-facing UART driver.
 * Depends only on HAL abstraction, not on specific hardware.
 * 
 * No STM32 HAL headers should be included here.
 */
```

---

## Extending the Framework

### Adding a New Driver

1. Create `drivers/xyz_driver.h` (interface)
2. Create `drivers/xyz_driver.c` (implementation)
3. Implement using only HAL interfaces
4. Add to `Makefile` C_SOURCES
5. Use in application layer

### Adding a Service

1. Create `services/xyz_service.h`
2. Create `services/xyz_service.c`
3. Initialize in `app_init()`
4. Add to `Makefile`

### Adding Interrupts

1. Define ISR in `platform/isr_handlers.c`
2. Reference in linker script
3. Call driver/service handlers from ISR
4. Keep ISR code minimal

---

## Project Maintenance

### Code Reviews

- [ ] No HAL leakage into app layer
- [ ] Proper error handling
- [ ] Clear dependencies
- [ ] Documentation updated
- [ ] Build succeeds with all HAL options
- [ ] No compiler warnings

### Build Verification

```bash
# Clean build
./build.sh clean && ./build.sh

# Test all HAL options
./build.sh HAL=stm32_hal MODE=debug
./build.sh HAL=stm32_hal MODE=release
# ./build.sh HAL=ll MODE=debug
# ./build.sh HAL=opencm3 MODE=debug
```

---

## References

- [Misra C Guidelines](https://www.misra.org.uk/)
- [Clean Architecture](https://blog.cleancoder.com/uncle-bob/2012/08/13/the-clean-architecture.html)
- [Embedded Systems Best Practices](https://www.barrgroup.com/embedded-systems/books/embedded-systems-dictionary)

---

**Framework Version:** 1.0  
**Last Updated:** 2026-01-02  
**Status:** Production-Ready
