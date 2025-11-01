#include "py/runtime.h"
#include "py/gc.h"
#include "py/mphal.h"
#include "py/lexer.h"     // 为了使用 mp_lexer_t
#include "py/builtin.h"   // 为了使用 mp_import_stat_t 和 MP_IMPORT_STAT_NO_EXIST
#include "cmsis_gcc.h"    // 为了使用 __get_MSP() 等 CMSIS 函数
#include <errno.h>        // 为了使用 ENOENT
#include <string.h>       // 为了使用 strlen

// ==================== Frozen Code 支持 ====================
// 不使用 frozen modules，所以不需要定义 mp_qstr_frozen_const_pool

// GC 收集实现
void gc_collect(void) {
    gc_collect_start();
    
    // 收集栈上的根指针
    // 注意：在 FreeRTOS 中，栈地址需要从任务控制块获取
    void *sp = (void*)__get_MSP();  // 或使用 FreeRTOS API
    extern uint8_t _sstack, _estack;
    gc_collect_root(sp, ((uint32_t)&_estack - (uint32_t)sp) / sizeof(uint32_t));
    
    gc_collect_end();
}

// 文件系统相关（暂不支持）
mp_lexer_t *mp_lexer_new_from_file(qstr filename) {
    mp_raise_OSError(ENOENT);
}

mp_import_stat_t mp_import_stat(const char *path) {
    return MP_IMPORT_STAT_NO_EXIST;
}

// ==================== Readline 支持 ====================
// MicroPython 自带的 readline.c 已经提供了完整实现，不需要我们自己实现

// ==================== builtin open 支持 ====================
// 简单的 open 实现（暂不支持文件系统）
mp_obj_t mp_builtin_open(size_t n_args, const mp_obj_t *args, mp_map_t *kwargs) {
    mp_raise_OSError(ENOENT);  // 文件系统未实现
}
MP_DEFINE_CONST_FUN_OBJ_KW(mp_builtin_open_obj, 1, mp_builtin_open);

// 异常处理
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