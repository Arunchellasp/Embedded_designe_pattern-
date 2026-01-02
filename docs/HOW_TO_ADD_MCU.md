# How to Add a New MCU/Board

This guide walks through adding support for a new STM32 microcontroller to the framework.

## Example: Adding STM32F407ZGT6 Support

### Step 1: Understand the Current Structure

The framework uses these files for board/MCU configuration:

```
bsp/board_config.h          ← Shared board definitions
bsp/bsp_clock.h/c           ← Clock configuration
bsp/bsp_init.h/c            ← Board initialization
boards/STM32F412ZET6/       ← Board-specific files (optional)
```

**Current board:** STM32F412ZET6 (256KB Flash, 192KB RAM, 100 MHz)  
**New board:** STM32F407ZGT6 (1MB Flash, 128KB RAM, 168 MHz)

---

### Step 2: Create Board Directory Structure

```bash
mkdir -p boards/STM32F407ZGT6
```

---

### Step 3: Define Board Configuration

**File: `bsp/board_config.h`**

Modify the existing file to support board selection:

```c
/*
 * board_config.h - Board Configuration Header
 */

#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

#include <stdint.h>

/* ===== BOARD SELECTION ===== */
#ifdef BOARD_STM32F407ZGT6
    #include "../boards/STM32F407ZGT6/board_config_f407.h"
#else
    /* Default to STM32F412ZET6 */
    #define BOARD_STM32F412ZET6
    #include "../boards/STM32F412ZET6/board_config_f412.h"
#endif

#endif /* BOARD_CONFIG_H */
```

---

### Step 4: Create Board-Specific Configuration Files

**File: `boards/STM32F412ZET6/board_config_f412.h`** (Move existing defines here)

```c
#ifndef BOARD_CONFIG_F412_H
#define BOARD_CONFIG_F412_H

#include <stdint.h>

/* ===== MCU SPECIFIC ===== */
#define MCU_STM32F412ZET6
#define FLASH_SIZE              0x40000     /* 256 KB */
#define RAM_SIZE                0x30000     /* 192 KB */

/* ===== CLOCK CONFIGURATION ===== */
#define SYSTEM_CLOCK_HZ         100000000UL  /* 100 MHz */
#define AHB_CLOCK_HZ            100000000UL
#define APB1_CLOCK_HZ           50000000UL
#define APB2_CLOCK_HZ           100000000UL

/* ===== LED PIN MAPPING ===== */
#define LED_PORT                GPIOB
#define LED_PIN                 0

/* ===== UART PINS ===== */
#define UART1_TX_PORT           GPIOA
#define UART1_TX_PIN            9
#define UART1_RX_PORT           GPIOA
#define UART1_RX_PIN            10

#endif /* BOARD_CONFIG_F412_H */
```

**File: `boards/STM32F407ZGT6/board_config_f407.h`** (New file)

```c
#ifndef BOARD_CONFIG_F407_H
#define BOARD_CONFIG_F407_H

#include <stdint.h>

/* ===== MCU SPECIFIC ===== */
#define MCU_STM32F407ZGT6
#define FLASH_SIZE              0x100000     /* 1 MB */
#define RAM_SIZE                0x20000      /* 128 KB */

/* ===== CLOCK CONFIGURATION ===== */
/* STM32F407 typically runs at 168 MHz */
#define SYSTEM_CLOCK_HZ         168000000UL
#define AHB_CLOCK_HZ            168000000UL
#define APB1_CLOCK_HZ           84000000UL   /* AHB/2 */
#define APB2_CLOCK_HZ           168000000UL

/* ===== LED PIN MAPPING ===== */
/* Different board may use different LED pin */
#define LED_PORT                GPIOA
#define LED_PIN                 5

/* ===== UART PINS ===== */
/* Some boards route UART differently */
#define UART1_TX_PORT           GPIOB
#define UART1_TX_PIN            6
#define UART1_RX_PORT           GPIOB
#define UART1_RX_PIN            7

#endif /* BOARD_CONFIG_F407_H */
```

---

### Step 5: Create MCU-Specific Clock Configuration

**File: `boards/STM32F407ZGT6/bsp_clock_f407.c`** (New file)

```c
/*
 * bsp_clock_f407.c - STM32F407 Clock Configuration
 */

#include "../../bsp/bsp_clock.h"
#include "board_config_f407.h"

static const clock_config_t f407_clock_config = {
    .system_clock_hz = SYSTEM_CLOCK_HZ,
    .ahb_clock_hz = AHB_CLOCK_HZ,
    .apb1_clock_hz = APB1_CLOCK_HZ,
    .apb2_clock_hz = APB2_CLOCK_HZ
};

error_t bsp_clock_init(void)
{
    /* STM32F407 Clock Configuration:
     * 
     * HSE: 8 MHz (typical crystal on STM32F4 Discovery boards)
     * PLL: M=8, N=336, P=2, Q=7
     *   Output: 8 * 336 / 8 / 2 = 168 MHz
     * 
     * AHB prescaler: 1
     * APB1 prescaler: 4 (84 MHz)
     * APB2 prescaler: 2 (168 MHz)
     */

    /* TODO: Implement actual clock configuration
     * 
     * STM32 HAL example:
     * 
     * RCC_OscInitTypeDef RCC_OscInitStruct = {0};
     * RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
     * 
     * RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
     * RCC_OscInitStruct.HSEState = RCC_HSE_ON;
     * RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
     * RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
     * RCC_OscInitStruct.PLL.PLLM = 8;
     * RCC_OscInitStruct.PLL.PLLN = 336;
     * RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
     * RCC_OscInitStruct.PLL.PLLQ = 7;
     * HAL_RCC_OscConfig(&RCC_OscInitStruct);
     * 
     * RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | 
     *                               RCC_CLOCKTYPE_SYSCLK |
     *                               RCC_CLOCKTYPE_PCLK1 |
     *                               RCC_CLOCKTYPE_PCLK2;
     * RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
     * RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
     * RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
     * RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
     * HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
     */

    return ERR_OK;
}

error_t bsp_clock_get_config(clock_config_t *config)
{
    if (config == NULL) {
        return ERR_INVALID_PARAM;
    }
    *config = f407_clock_config;
    return ERR_OK;
}

uint32_t bsp_clock_get_system_clock(void)
{
    return f407_clock_config.system_clock_hz;
}

uint32_t bsp_clock_get_ahb_clock(void)
{
    return f407_clock_config.ahb_clock_hz;
}

uint32_t bsp_clock_get_apb1_clock(void)
{
    return f407_clock_config.apb1_clock_hz;
}

uint32_t bsp_clock_get_apb2_clock(void)
{
    return f407_clock_config.apb2_clock_hz;
}
```

---

### Step 6: Modify Makefile for Board Selection

**File: `Makefile`** (Update C_SOURCES section)

```makefile
# ===== SOURCE FILES =====
C_SOURCES := \
    main.c \
    common/error.c \
    app/app.c \
    drivers/gpio_driver.c \
    drivers/uart_driver.c \
    hal/hal_gpio.c \
    hal/hal_uart.c \
    bsp/bsp_init.c

# Add board-specific clock implementation
ifeq ($(BOARD), STM32F412ZET6)
    C_SOURCES += boards/STM32F412ZET6/bsp_clock_f412.c
else ifeq ($(BOARD), STM32F407ZGT6)
    C_SOURCES += boards/STM32F407ZGT6/bsp_clock_f407.c
endif

C_SOURCES += platform/platform_startup.c
```

**Update CFLAGS for board definition:**

```makefile
# ===== BOARD SELECTION DEFINES =====
CFLAGS += -DBOARD_$(BOARD)
```

---

### Step 7: Create Linker Script (if needed)

**File: `platform/linker_f407.ld`** (New file)

```ld
/* STM32F407ZGT6 Linker Script */

MEMORY
{
  FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 1M
  RAM (rwx)   : ORIGIN = 0x20000000, LENGTH = 128K
}

SECTIONS
{
  .text :
  {
    KEEP(*(.vectors))
    *(.text*)
    *(.rodata*)
  } > FLASH

  .data :
  {
    *(.data*)
  } > RAM AT > FLASH

  .bss :
  {
    *(.bss*)
  } > RAM
}
```

Update `Makefile` to select linker script:

```makefile
ifeq ($(BOARD), STM32F407ZGT6)
    LDSCRIPT := platform/linker_f407.ld
else
    LDSCRIPT := platform/linker_f412.ld
endif
```

---

### Step 8: Build for New MCU

```bash
# Build for STM32F407ZGT6
./build.sh BOARD=STM32F407ZGT6

# With specific HAL
./build.sh BOARD=STM32F407ZGT6 HAL=ll MODE=release

# Check configuration
./build.sh BOARD=STM32F407ZGT6 info
```

---

### Step 9: Test and Verify

```bash
# Check for compiler errors
./build.sh BOARD=STM32F407ZGT6

# Check binary size
make BOARD=STM32F407ZGT6 size

# Generate map file
arm-none-eabi-objdump -d build/STM32F407ZGT6/debug/output/embedded_firmware.elf > fw.asm
```

---

## Checklist for Adding New MCU

- [ ] Create board directory: `boards/<MCU_NAME>/`
- [ ] Create board config header: `board_config_<mcu_shortname>.h`
- [ ] Create clock configuration: `bsp_clock_<mcu_shortname>.c`
- [ ] Define pin mappings for your board
- [ ] Update `Makefile` board selection
- [ ] Create linker script (if different from existing)
- [ ] Build and verify: `./build.sh BOARD=<MCU_NAME>`
- [ ] Test all HAL options
- [ ] Test debug and release builds
- [ ] Document board specifics in README

---

## Adding a Different Board with Same MCU

If you have multiple boards with the same MCU but different pin layouts:

```bash
mkdir boards/STM32F407ZGT6_V2
# Copy and modify board_config_f407.h
# Update pin definitions for your variant
```

Example:

```c
// boards/STM32F407ZGT6_V2/board_config_f407_v2.h
#define LED_PORT                GPIOC
#define LED_PIN                 13
```

Then update `bsp/board_config.h`:

```c
#elif defined(BOARD_STM32F407ZGT6_V2)
    #include "../boards/STM32F407ZGT6_V2/board_config_f407_v2.h"
```

Build:

```bash
./build.sh BOARD=STM32F407ZGT6_V2
```

---

## Common MCU Configuration Parameters

### STM32F4 Series Clock Setup

```c
/* Common crystal frequencies */
#define HSE_VALUE       8000000UL  /* 8 MHz (Discovery boards) */
#define HSE_VALUE_ALT   12000000UL /* 12 MHz (some custom boards) */

/* Typical clock targets */
#define F407_SYSCLOCK   168000000UL
#define F429_SYSCLOCK   180000000UL
#define F446_SYSCLOCK   180000000UL
```

### Flash Wait States

```
Clock (MHz) | Wait States (Voltage 2.7V-3.6V)
0-30        | 0
30-60       | 1
60-90       | 2
90-120      | 3
120-150     | 4
150-168     | 5
```

---

## Troubleshooting

### "undefined reference to `bsp_clock_init`"

Make sure your new clock file is in `Makefile` C_SOURCES:

```makefile
C_SOURCES += boards/STM32F407ZGT6/bsp_clock_f407.c
```

### "multiple definition of `bsp_clock_init`"

Remove the old `bsp/bsp_clock.c` from C_SOURCES if using board-specific version.

### Wrong clock frequency

Update `board_config_<mcu>.h` with correct SYSTEM_CLOCK_HZ and verify PLL setup in clock init code.

---

## Reference

- **STM32F412 Datasheet:** ARM Cortex-M4, 100 MHz max, 256KB Flash
- **STM32F407 Datasheet:** ARM Cortex-M4, 168 MHz max, 1MB Flash
- **STM32 HAL Documentation:** Available with STM32CubeMX

---

**Framework Version:** 1.0  
**Last Updated:** 2026-01-02
