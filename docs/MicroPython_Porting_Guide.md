# MicroPython Porting Guide for STM32F407VET6 with FreeRTOS

## Overview

This document describes the complete process of porting MicroPython to STM32F407VET6 microcontroller with FreeRTOS integration. The final implementation provides a fully functional REPL (Read-Eval-Print Loop) with readline support, command history, and UART communication.

## System Requirements

- **Hardware**: STM32F407VET6 development board
- **RTOS**: FreeRTOS
- **Build System**: CMake
- **Toolchain**: ARM GCC (arm-none-eabi-gcc)
- **MicroPython Version**: Latest from GitHub

## Memory Configuration

- **Flash**: 512 KB (40.82% used ~214 KB)
- **RAM**: 128 KB (76.56% used ~98 KB)
- **CCM RAM**: 64 KB (not used)
- **MicroPython Heap**: 32 KB
- **MicroPython Stack**: 8 KB
- **FreeRTOS Task Stack**: 16 KB (4096 * 4 bytes)

## Project Structure

```
stm32f407vet6-micropython/
├── MicroPython/
│   ├── py/                      # MicroPython core
│   ├── shared/                  # Shared utilities (readline, runtime)
│   ├── micropython_port/        # Port-specific implementation
│   │   ├── mpconfigport.h       # MicroPython configuration
│   │   ├── mpconfigport.c       # Port implementation
│   │   ├── mphalport.h          # HAL header
│   │   ├── mphalport.c          # HAL implementation
│   │   └── qstrdefsport.h       # Port-specific qstrings
│   └── CMakeLists.txt           # MicroPython build configuration
├── Core/
│   └── Src/
│       ├── freertos.c           # FreeRTOS tasks (including MicroPython task)
│       └── main.c               # Main application
└── docs/
    └── MicroPython_Porting_Guide.md  # This document
```

## Step-by-Step Porting Process

### 1. Initial CMake Configuration

#### Problem
Initial build failed with error:
```
fatal error: genhdr/qstrdefs.generated.h: No such file or directory
```

#### Solution
Configure MicroPython's qstr generation system in `MicroPython/CMakeLists.txt`:

```cmake
# Define MicroPython directories
set(MICROPY_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(MICROPY_PY_DIR ${MICROPY_DIR}/py)
set(MICROPY_SHARED_DIR ${MICROPY_DIR}/shared)
set(MICROPY_PORT_DIR ${MICROPY_DIR}/micropython_port)
set(MICROPY_TARGET stm32f407vet6-micropython)

# Define qstr port definitions
set(MICROPY_QSTRDEFS_PORT ${MICROPY_PORT_DIR}/qstrdefsport.h)

# Define source files for qstr scanning
set(MICROPY_SOURCE_QSTR
    # All py/*.c files
    # Add shared files containing root pointers
    ${MICROPY_SHARED_DIR}/readline/readline.c
    ${MICROPY_SHARED_DIR}/runtime/pyexec.c
)

# Include MicroPython's build rules
include(${MICROPY_PY_DIR}/mkrules.cmake)
```

### 2. Header File Inclusion Order

#### Problem
STM32 HAL types (`__IO`, `IRQn_Type`, `TIM_TypeDef`) were undefined during compilation.

#### Solution
Fix header inclusion order in `mpconfigport.h`:

```c
#ifndef MICROPY_INCLUDED_MPCONFIGPORT_H
#define MICROPY_INCLUDED_MPCONFIGPORT_H

// ==================== STM32 & CMSIS Headers ====================
// Must be included first to ensure all base types are defined
#include <stdint.h>

// Define STM32 device type (before including stm32f4xx.h)
#ifndef STM32F407xx
#define STM32F407xx
#endif

#include "stm32f4xx.h"  // STM32F4 device header

// ==================== Standard Library ====================
#include <alloca.h>
#include <stdio.h>

// ... rest of configuration
```

### 3. FreeRTOS Integration

#### Problem
FreeRTOS header inclusion order error:
```
#error "include FreeRTOS.h must appear in source files before include task.h"
```

#### Solution
Fix inclusion order in `mphalport.h`:

```c
#ifndef MICROPY_INCLUDED_MPHAL_H
#define MICROPY_INCLUDED_MPHAL_H

#include "py/mphal.h"
#include "FreeRTOS.h"  // Must come before task.h
#include "task.h"

// ... rest of header
```

Create MicroPython task in `freertos.c`:

```c
// MicroPython heap memory (static allocation)
static char micropython_heap[32 * 1024];  // 32KB

void MicroPythonTask(void *argument) {
    // Wait for system stabilization
    osDelay(100);
    
    const char *msg = "\r\n=== MicroPython Starting ===\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
    
    // Initialize MicroPython
    gc_init(micropython_heap, micropython_heap + sizeof(micropython_heap));
    
    msg = "GC initialized\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
    
    mp_init();
    
    msg = "MP initialized\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
    
    // Start REPL
    for (;;) {
        if (pyexec_mode_kind == PYEXEC_MODE_RAW_REPL) {
            if (pyexec_raw_repl() != 0) {
                break;
            }
        } else {
            if (pyexec_friendly_repl() != 0) {
                break;
            }
        }
    }
    
    // Cleanup
    mp_deinit();
    osThreadTerminate(NULL);
}

// Task attributes
const osThreadAttr_t micropython_task_attributes = {
    .name = "MicroPython",
    .stack_size = 4096 * 4,  // 16KB stack
    .priority = (osPriority_t) osPriorityNormal,
};
```

### 4. UART HAL Implementation

Implement HAL functions in `mphalport.c`:

```c
#include "mphalport.h"
#include "main.h"
#include <stdio.h>

extern UART_HandleTypeDef huart1;

// Receive character (blocking)
int mp_hal_stdin_rx_chr(void) {
    uint8_t c = 0;
    HAL_UART_Receive(&huart1, &c, 1, HAL_MAX_DELAY);
    return c;
}

// Send string
void mp_hal_stdout_tx_strn(const char *str, size_t len) {
    HAL_UART_Transmit(&huart1, (uint8_t*)str, len, HAL_MAX_DELAY);
}

// Interrupt character setting
static int mp_interrupt_char = -1;

void mp_hal_set_interrupt_char(int c) {
    mp_interrupt_char = c;
}
```

### 5. Readline Support and Root Pointers

#### Problem
Readline history support required `readline_hist` to be registered as a root pointer.

#### Solution A: Enable readline.c in build
Add to `MicroPython/CMakeLists.txt`:

```cmake
set(MICROPY_SHARED_SRC
    # libc
    ${MICROPY_SHARED_DIR}/libc/printf.c
    ${MICROPY_SHARED_DIR}/libc/string0.c
    
    # readline
    ${MICROPY_SHARED_DIR}/readline/readline.c
    
    # runtime
    ${MICROPY_SHARED_DIR}/runtime/pyexec.c
    ${MICROPY_SHARED_DIR}/runtime/stdout_helpers.c
    ${MICROPY_SHARED_DIR}/runtime/sys_stdio_mphal.c
    ${MICROPY_SHARED_DIR}/runtime/interrupt_char.c
    ${MICROPY_SHARED_DIR}/runtime/gchelper_thumb2.s
)
```

#### Solution B: Add readline.c to qstr scanning
This ensures `MP_REGISTER_ROOT_POINTER` in readline.c is discovered:

```cmake
set(MICROPY_SOURCE_QSTR
    # ... all py/*.c files ...
    # Add shared files containing root pointers
    ${MICROPY_SHARED_DIR}/readline/readline.c
    ${MICROPY_SHARED_DIR}/runtime/pyexec.c
)
```

#### Solution C: Configure readline in mpconfigport.h

```c
// ==================== REPL Configuration ====================
#define MICROPY_HELPER_LEXER_UNIX           (1)
#define MICROPY_HELPER_INPUT_HISTORY        (1)  // Enable history
#define MICROPY_READLINE_HISTORY_SIZE       (8)  // History entries

// ==================== Port State ====================
// Root pointers managed by MP_REGISTER_ROOT_POINTER
#define MP_STATE_PORT MP_STATE_VM
```

### 6. Port-Specific Implementation

Implement required functions in `mpconfigport.c`:

```c
#include "py/runtime.h"
#include "py/gc.h"
#include "py/mphal.h"
#include "py/lexer.h"
#include "py/builtin.h"
#include "cmsis_gcc.h"
#include <errno.h>
#include <string.h>

// GC collection implementation
void gc_collect(void) {
    gc_collect_start();
    
    // Collect stack root pointers
    void *sp = (void*)__get_MSP();
    extern uint8_t _sstack, _estack;
    gc_collect_root(sp, ((uint32_t)&_estack - (uint32_t)sp) / sizeof(uint32_t));
    
    gc_collect_end();
}

// File system (not supported)
mp_lexer_t *mp_lexer_new_from_file(qstr filename) {
    mp_raise_OSError(ENOENT);
}

mp_import_stat_t mp_import_stat(const char *path) {
    return MP_IMPORT_STAT_NO_EXIST;
}

// builtin open (not supported)
mp_obj_t mp_builtin_open(size_t n_args, const mp_obj_t *args, mp_map_t *kwargs) {
    mp_raise_OSError(ENOENT);
}
MP_DEFINE_CONST_FUN_OBJ_KW(mp_builtin_open_obj, 1, mp_builtin_open);

// Exception handling
void nlr_jump_fail(void *val) {
    for (;;) {
        __WFI();
    }
}

void __fatal_error(const char *msg) {
    for (;;) {
        __WFI();
    }
}

#ifndef NDEBUG
void __assert_func(const char *file, int line, const char *func, const char *expr) {
    printf("Assertion '%s' failed, at file %s:%d\n", expr, file, line);
    __fatal_error("Assertion failed");
}
#endif
```

### 7. MicroPython Configuration

Complete `mpconfigport.h` configuration:

```c
// ==================== Core Features ====================
#define MICROPY_CONFIG_ROM_LEVEL            (MICROPY_CONFIG_ROM_LEVEL_CORE_FEATURES)
#define MICROPY_ENABLE_COMPILER             (1)
#define MICROPY_ENABLE_GC                   (1)
#define MICROPY_HELPER_REPL                 (1)

// ==================== Built-in Functions ====================
#define MICROPY_PY_BUILTINS_HELP            (1)  // Enable help() function

// ==================== REPL Configuration ====================
#define MICROPY_HELPER_LEXER_UNIX           (1)
#define MICROPY_HELPER_INPUT_HISTORY        (1)
#define MICROPY_READLINE_HISTORY_SIZE       (8)

// ==================== Memory Configuration ====================
#define MICROPY_HEAP_SIZE                   (32 * 1024)  // 32KB
#define MICROPY_STACK_SIZE                  (8 * 1024)   // 8KB

// ==================== Feature Switches ====================
#define MICROPY_PY_THREAD                   (0)  // Thread support
#define MICROPY_ENABLE_SCHEDULER            (0)  // Scheduler support
#define MICROPY_ENABLE_PYSTACK              (0)  // Python stack support

// ==================== Board Information ====================
#define MICROPY_HW_BOARD_NAME               "STM32-FreeRTOS"
#define MICROPY_HW_MCU_NAME                 "STM32F4"

// ==================== Type Definitions ====================
typedef long mp_off_t;

// ==================== Port State ====================
#define MP_STATE_PORT MP_STATE_VM
```

### 8. Qstr Port Definitions

Create `qstrdefsport.h` for port-specific strings:

```c
// Port-specific qstr definitions
Q(readlines)
Q(TextIOWrapper)
```

### 9. Build Configuration

#### Key CMake Settings

```cmake
# Compilation definitions
target_compile_definitions(micropython PUBLIC
    # No frozen modules
    MICROPY_MODULE_FROZEN_MPY=0
)

# MicroPython-specific compiler options
target_compile_options(micropython PRIVATE
    -Wno-unused-parameter
    -Wno-missing-field-initializers
    -Wno-sign-compare
)

# Include directories
target_include_directories(micropython PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${MICROPY_PY_DIR}
    ${MICROPY_PORT_DIR}
    ${CMAKE_BINARY_DIR}  # For generated headers
    # STM32 HAL and CMSIS
    ${CMAKE_SOURCE_DIR}/Core/Inc
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Inc
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32F4xx/Include
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Include
    # FreeRTOS
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/include
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
)
```

## Common Issues and Solutions

### Issue 1: Qstr Pool Assertion Error
**Error**: `Assertion '*q < pool->len' failed`

**Cause**: Incorrect `MICROPY_QSTR_EXTRA_POOL` configuration when `MICROPY_MODULE_FROZEN_MPY=0`

**Solution**: Remove `MICROPY_QSTR_EXTRA_POOL` definition when not using frozen modules

### Issue 2: No REPL Prompt
**Error**: No `>>>` prompt appears

**Cause**: Custom readline implementation with wrong signature

**Solution**: Use MicroPython's built-in `readline.c` instead of custom implementation

### Issue 3: Missing readline_hist
**Error**: `'mp_state_vm_t' has no member named 'readline_hist'`

**Cause**: readline.c not included in qstr scanning, so `MP_REGISTER_ROOT_POINTER` not discovered

**Solution**: Add readline.c to `MICROPY_SOURCE_QSTR` list

### Issue 4: FreeRTOS Stack Overflow
**Symptom**: System crashes or hangs

**Cause**: Insufficient stack size for MicroPython task

**Solution**: Increase stack size to at least 16KB:
```c
.stack_size = 4096 * 4,  // 16KB stack
```

## Testing and Verification

### 1. Basic REPL Test
```python
>>> print("Hello MicroPython!")
Hello MicroPython!
>>> 1 + 1
2
>>> for i in range(5):
...     print(i)
... 
0
1
2
3
4
```

### 2. Help Function Test
```python
>>> help()
Welcome to MicroPython!
...
```

### 3. Memory Test
```python
>>> import gc
>>> gc.mem_free()
28672
>>> gc.collect()
>>> gc.mem_alloc()
3456
```

### 4. Command History
- Use UP/DOWN arrow keys to navigate history
- Maximum 8 history entries (configurable)

## Expected Output

```
=== STM32 System Started ===

=== MicroPython Starting ===
GC initialized
MP initialized
MicroPython c4504b8289-dirty on 2025-11-01; STM32-FreeRTOS with STM32F4
Type "help()" for more information.
>>> 
```

## Performance Characteristics

- **Boot Time**: ~100ms (including FreeRTOS initialization)
- **REPL Response**: Immediate for simple commands
- **Memory Usage**: 
  - Idle: ~3-4 KB allocated
  - After GC: ~28 KB free
- **Flash Usage**: ~214 KB (40.82% of 512 KB)
- **RAM Usage**: ~98 KB (76.56% of 128 KB)

## Future Enhancements

1. **Hardware Abstraction**
   - Implement `machine` module for GPIO, I2C, SPI access
   - Add PWM and ADC support
   - Timer and interrupt support

2. **File System**
   - Add littlefs for flash storage
   - Implement VFS (Virtual File System)
   - Support for frozen Python modules

3. **Network**
   - Add network stack support
   - WebREPL implementation
   - Socket API

4. **Optimization**
   - Enable native code emission
   - Optimize memory usage
   - Add pystack support

## References

- [MicroPython Official Documentation](https://docs.micropython.org/)
- [MicroPython GitHub Repository](https://github.com/micropython/micropython)
- [STM32F407 Reference Manual](https://www.st.com/resource/en/reference_manual/dm00031020.pdf)
- [FreeRTOS Documentation](https://www.freertos.org/Documentation/RTOS_book.html)

## Conclusion

This guide provides a complete walkthrough of porting MicroPython to STM32F407VET6 with FreeRTOS. The implementation includes a fully functional REPL with readline support, proper memory management, and integration with the FreeRTOS task scheduler. The modular structure allows for easy expansion with additional MicroPython modules and hardware support.

## Revision History

- **2025-11-01**: Initial version - Complete port with REPL, readline, and FreeRTOS integration

