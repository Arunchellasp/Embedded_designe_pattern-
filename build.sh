#!/bin/bash

###############################################################################
# Industrial Embedded Firmware Build Script
# 
# Supports multiple boards, HAL implementations, and build modes
# Usage: ./build.sh [BOARD=...] [HAL=...] [MODE=...]
###############################################################################

set -e  # Exit on error

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_NAME="embedded_firmware"

# ===== DEFAULT CONFIGURATION =====
BOARD="${BOARD:-STM32F412ZET6}"
HAL="${HAL:-stm32_hal}"
MODE="${MODE:-debug}"
JOBS="${JOBS:-$(nproc)}"

# ===== COLORS =====
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'  # No Color

# ===== FUNCTIONS =====

print_header() {
    echo -e "${BLUE}=========================================${NC}"
    echo -e "${BLUE}$1${NC}"
    echo -e "${BLUE}=========================================${NC}"
}

print_success() {
    echo -e "${GREEN}✓ $1${NC}"
}

print_error() {
    echo -e "${RED}✗ $1${NC}"
}

print_info() {
    echo -e "${YELLOW}ℹ $1${NC}"
}

show_help() {
    cat << EOF
Industrial Embedded Firmware Build Script

Usage: ./build.sh [OPTIONS]

Options:
  BOARD=<name>     Target board (default: STM32F412ZET6)
  HAL=<type>       HAL implementation (default: stm32_hal)
                   Options: stm32_hal, ll, opencm3
  MODE=<mode>      Build mode (default: debug)
                   Options: debug, release
  JOBS=<n>         Number of parallel build jobs (default: auto-detect)
  
Special Targets:
  ./build.sh clean           Clean build artifacts
  ./build.sh distclean       Deep clean (build + intermediate files)
  ./build.sh info            Show configuration
  ./build.sh help            Show this help

Examples:
  ./build.sh
  ./build.sh BOARD=STM32F412ZET6 HAL=stm32_hal MODE=release
  ./build.sh clean
  ./build.sh info

EOF
}

validate_board() {
    case "$1" in
        STM32F412ZET6)
            print_info "Board: $1 (STM32F412ZET6 - 256KB Flash, 192KB RAM)"
            ;;
        *)
            print_error "Unknown board: $1"
            echo "Supported boards:"
            echo "  - STM32F412ZET6"
            exit 1
            ;;
    esac
}

validate_hal() {
    case "$1" in
        stm32_hal)
            print_info "HAL: STM32 HAL (STM32Cube)"
            ;;
        ll)
            print_info "HAL: STM32 LL (Low-Level)"
            ;;
        opencm3)
            print_info "HAL: libopencm3"
            ;;
        *)
            print_error "Unknown HAL: $1"
            echo "Supported HALs:"
            echo "  - stm32_hal (STM32Cube)"
            echo "  - ll        (STM32 Low-Level)"
            echo "  - opencm3   (libopencm3)"
            exit 1
            ;;
    esac
}

validate_mode() {
    case "$1" in
        debug)
            print_info "Mode: Debug (with symbols, no optimization)"
            ;;
        release)
            print_info "Mode: Release (optimized, no symbols)"
            ;;
        *)
            print_error "Unknown mode: $1"
            echo "Supported modes:"
            echo "  - debug    (Development)"
            echo "  - release  (Production)"
            exit 1
            ;;
    esac
}

build_project() {
    print_header "Building $PROJECT_NAME"
    print_info "Configuration: BOARD=$BOARD, HAL=$HAL, MODE=$MODE"
    
    cd "$SCRIPT_DIR"
    
    # Run make with parallel jobs
    if make -j"$JOBS" BOARD="$BOARD" HAL="$HAL" MODE="$MODE"; then
        print_success "Build completed successfully"
        
        # Show build information
        BUILD_DIR="build/$BOARD/$MODE/output"
        if [ -f "$BUILD_DIR/${PROJECT_NAME}.elf" ]; then
            print_info "Output files:"
            echo "  ELF:  $BUILD_DIR/${PROJECT_NAME}.elf"
            echo "  BIN:  $BUILD_DIR/${PROJECT_NAME}.bin"
            echo "  MAP:  $BUILD_DIR/${PROJECT_NAME}.map"
            
            # Show size information
            print_info "Binary size:"
            arm-none-eabi-size "$BUILD_DIR/${PROJECT_NAME}.elf" 2>/dev/null || true
        fi
    else
        print_error "Build failed"
        exit 1
    fi
}

# ===== MAIN SCRIPT =====

# Parse arguments
case "${1:-}" in
    clean)
        make clean BOARD="$BOARD" HAL="$HAL" MODE="$MODE"
        print_success "Clean completed"
        exit 0
        ;;
    distclean)
        rm -rf build/
        print_success "Distclean completed"
        exit 0
        ;;
    info)
        validate_board "$BOARD"
        validate_hal "$HAL"
        validate_mode "$MODE"
        make info BOARD="$BOARD" HAL="$HAL" MODE="$MODE"
        exit 0
        ;;
    help|--help|-h)
        show_help
        exit 0
        ;;
    "")
        # Default: build
        ;;
    *)
        print_error "Unknown target: $1"
        echo "Run './build.sh help' for usage information"
        exit 1
        ;;
esac

# Validate configuration
echo ""
print_header "Build Configuration"
validate_board "$BOARD"
validate_hal "$HAL"
validate_mode "$MODE"
echo ""

# Check toolchain
print_header "Checking Toolchain"
if command -v arm-none-eabi-gcc &> /dev/null; then
    GCC_VERSION=$(arm-none-eabi-gcc --version | head -n1)
    print_success "Toolchain found: $GCC_VERSION"
else
    print_error "ARM toolchain not found (arm-none-eabi-gcc)"
    echo "Please install: apt-get install gcc-arm-none-eabi"
    exit 1
fi
echo ""

# Build
build_project

echo ""
print_success "All done!"
