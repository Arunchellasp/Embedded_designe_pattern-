# ============================================================================
# INDUSTRIAL-GRADE EMBEDDED FIRMWARE BUILD SYSTEM
# 
# Target: STM32F412ZET6
# Supports multiple HAL implementations
# ============================================================================

# ===== BUILD CONFIGURATION =====
PROJECT_NAME := embedded_firmware
BOARD ?= STM32F412ZET6
HAL ?= stm32_hal
MODE ?= debug

# ===== TOOLCHAIN =====
CROSS_COMPILE ?= arm-none-eabi-
CC := $(CROSS_COMPILE)gcc
AR := $(CROSS_COMPILE)ar
OBJCOPY := $(CROSS_COMPILE)objcopy
OBJDUMP := $(CROSS_COMPILE)objdump
SIZE := $(CROSS_COMPILE)size

# ===== DIRECTORIES =====
ROOT_DIR := $(PWD)
SRC_DIR := $(ROOT_DIR)
BUILD_DIR := $(ROOT_DIR)/build/$(BOARD)/$(MODE)
OUTPUT_DIR := $(BUILD_DIR)/output
OBJ_DIR := $(BUILD_DIR)/obj

# ===== SOURCE FILES =====
C_SOURCES := \
	main.c \
	common/error.c \
	app/app.c \
	drivers/gpio_driver.c \
	drivers/uart_driver.c \
	hal/hal_gpio.c \
	hal/hal_uart.c \
	bsp/bsp_init.c \
	bsp/bsp_clock.c \
	platform/platform_startup.c

# ===== INCLUDE PATHS =====
INC_PATHS := \
	-I$(SRC_DIR) \
	-I$(SRC_DIR)/common \
	-I$(SRC_DIR)/app \
	-I$(SRC_DIR)/drivers \
	-I$(SRC_DIR)/hal \
	-I$(SRC_DIR)/bsp \
	-I$(SRC_DIR)/platform

# ===== COMPILER FLAGS =====
COMMON_FLAGS := \
	$(INC_PATHS) \
	-std=c11 \
	-Wall -Wextra -Werror \
	-ffunction-sections -fdata-sections \
	-fno-common \
	-march=armv7-m \
	-mcpu=cortex-m4 \
	-mthumb

CFLAGS := $(COMMON_FLAGS) \
	-Wbad-function-cast \
	-Wcast-align \
	-Wconversion \
	-Wshadow \
	-Wstrict-prototypes \
	-Wundef

# ===== DEBUG FLAGS =====
ifeq ($(MODE), debug)
	CFLAGS += -g3 -O0 -DDEBUG
	LDFLAGS := -Wl,-Map=$(OUTPUT_DIR)/$(PROJECT_NAME).map,--cref,--gc-sections
else
	CFLAGS += -O2 -DRELEASE
	LDFLAGS := -Wl,-Map=$(OUTPUT_DIR)/$(PROJECT_NAME).map,--cref,--gc-sections
endif

# ===== HAL SELECTION =====
ifeq ($(HAL), stm32_hal)
	CFLAGS += -DUSE_STM32_HAL
else ifeq ($(HAL), ll)
	CFLAGS += -DUSE_STM32_LL
else ifeq ($(HAL), opencm3)
	CFLAGS += -DUSE_OPENCM3
endif

# ===== OBJECT FILES =====
OBJS := $(C_SOURCES:%.c=$(OBJ_DIR)/%.o)
DEPS := $(OBJS:%.o=%.d)

# ===== OUTPUT FILES =====
ELF := $(OUTPUT_DIR)/$(PROJECT_NAME).elf
BIN := $(OUTPUT_DIR)/$(PROJECT_NAME).bin
MAP := $(OUTPUT_DIR)/$(PROJECT_NAME).map

# ===== RULES =====

.PHONY: all clean info help

all: $(ELF) $(BIN) size

$(OUTPUT_DIR):
	@mkdir -p $(OUTPUT_DIR)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/common
	@mkdir -p $(OBJ_DIR)/app
	@mkdir -p $(OBJ_DIR)/drivers
	@mkdir -p $(OBJ_DIR)/hal
	@mkdir -p $(OBJ_DIR)/bsp
	@mkdir -p $(OBJ_DIR)/platform

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@
	@echo "CC: $<"

$(ELF): $(OBJS) | $(OUTPUT_DIR)
	$(CC) $(OBJS) $(LDFLAGS) -o $@
	@echo "LD: $(notdir $@)"

$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@
	@echo "OBJCOPY: $(notdir $@)"

size: $(ELF)
	@$(SIZE) $(ELF)

info:
	@echo "========================================="
	@echo "PROJECT: $(PROJECT_NAME)"
	@echo "BOARD: $(BOARD)"
	@echo "HAL: $(HAL)"
	@echo "MODE: $(MODE)"
	@echo "========================================="
	@echo "Build directory: $(BUILD_DIR)"
	@echo "Output: $(ELF)"
	@echo "========================================="

clean:
	@rm -rf $(BUILD_DIR)
	@echo "Clean: $(BUILD_DIR)"

help:
	@echo "Industrial Embedded Firmware Build System"
	@echo ""
	@echo "Usage: make [OPTIONS]"
	@echo ""
	@echo "Options:"
	@echo "  BOARD=<board>    Target board (default: STM32F412ZET6)"
	@echo "  HAL=<hal>        HAL implementation (default: stm32_hal)"
	@echo "                   Options: stm32_hal, ll, opencm3"
	@echo "  MODE=<mode>      Build mode (default: debug)"
	@echo "                   Options: debug, release"
	@echo ""
	@echo "Targets:"
	@echo "  all              Build firmware (default)"
	@echo "  clean            Clean build artifacts"
	@echo "  info             Show build configuration"
	@echo "  help             Show this help message"
	@echo ""
	@echo "Examples:"
	@echo "  make"
	@echo "  make BOARD=STM32F412ZET6 HAL=stm32_hal MODE=release"
	@echo "  make clean"

-include $(DEPS)
