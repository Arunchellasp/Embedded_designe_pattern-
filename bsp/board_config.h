/*
 * board_config.h - Board Configuration Header
 * 
 * Defines board-specific configurations and selects the correct BSP.
 * NO HAL headers should be included here - only defines and typedefs.
 */

#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

#include <stdint.h>

/* ===== BOARD SELECTION ===== */
#define BOARD_STM32F412ZET6

/* ===== CLOCK CONFIGURATION ===== */
#define SYSTEM_CLOCK_HZ         100000000UL  /* 100 MHz */
#define AHB_CLOCK_HZ            100000000UL
#define APB1_CLOCK_HZ           50000000UL   /* APB1 = AHB/2 */
#define APB2_CLOCK_HZ           100000000UL  /* APB2 = AHB */

/* ===== LED PIN MAPPING ===== */
#define LED_PORT                GPIOB
#define LED_PIN                 0

/* ===== UART PINS ===== */
#define UART1_TX_PORT           GPIOA
#define UART1_TX_PIN            9
#define UART1_RX_PORT           GPIOA
#define UART1_RX_PIN            10

#define UART2_TX_PORT           GPIOA
#define UART2_TX_PIN            2
#define UART2_RX_PORT           GPIOA
#define UART2_RX_PIN            3

/* ===== MCU SPECIFIC ===== */
#define MCU_STM32F412ZET6
#define FLASH_SIZE              0x40000     /* 256 KB */
#define RAM_SIZE                0x30000     /* 192 KB */

#endif /* BOARD_CONFIG_H */
