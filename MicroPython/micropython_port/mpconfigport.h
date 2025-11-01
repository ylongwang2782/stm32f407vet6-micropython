#ifndef MICROPY_INCLUDED_MPCONFIGPORT_H
#define MICROPY_INCLUDED_MPCONFIGPORT_H

// ==================== STM32 和 CMSIS 头文件 ====================
// 必须在最前面包含，确保所有基础类型（__IO, IRQn_Type 等）都被定义
#include <stdint.h>

// 定义 STM32 设备类型（必须在包含 stm32f4xx.h 之前）
#ifndef STM32F407xx
#define STM32F407xx
#endif

#include "stm32f4xx.h"  // STM32F4 设备头文件

// ==================== 标准库包含 ====================
#include <alloca.h>
#include <stdio.h>

// ==================== FreeRTOS 相关配置 ====================
#define MICROPY_CONFIG_ROM_LEVEL            (MICROPY_CONFIG_ROM_LEVEL_CORE_FEATURES)
#define MICROPY_ENABLE_COMPILER             (1)
#define MICROPY_ENABLE_GC                   (1)
#define MICROPY_HELPER_REPL                 (1)

// ==================== REPL 相关 ====================
#define MICROPY_HELPER_LEXER_UNIX           (1)
#define MICROPY_HELPER_INPUT_HISTORY        (0)  // 暂时关闭历史支持

// ==================== 内存配置 ====================
// 堆大小 - 根据你的 RAM 调整
#define MICROPY_HEAP_SIZE                   (32 * 1024)  // 32KB

// 堆栈大小 - 用于 MicroPython 执行
#define MICROPY_STACK_SIZE                  (8 * 1024)   // 8KB

// ==================== 功能开关 ====================
// 启用线程支持（如果需要与 FreeRTOS 集成）
#define MICROPY_PY_THREAD                   (0)  // 暂时关闭

// 启用特定功能
#define MICROPY_ENABLE_SCHEDULER            (0)  // 调度器支持
#define MICROPY_ENABLE_PYSTACK              (0)  // Python 栈支持

// ==================== 板级信息 ====================
#define MICROPY_HW_BOARD_NAME               "STM32-FreeRTOS"
#define MICROPY_HW_MCU_NAME                 "STM32F4"

// ==================== 类型定义 ====================
typedef long mp_off_t;

// ==================== Port State 定义 ====================
// 如果启用了 readline 历史支持，需要定义状态存储
#if MICROPY_HELPER_INPUT_HISTORY
#define MICROPY_PORT_ROOT_POINTERS \
    const char *readline_hist[MICROPY_READLINE_HISTORY_SIZE];
#endif

#define MP_STATE_PORT MP_STATE_VM

#endif