#ifndef MICROPY_INCLUDED_MPHALPORT_H
#define MICROPY_INCLUDED_MPHALPORT_H

#include "py/mpconfig.h"
#include "FreeRTOS.h"  // 必须在 task.h 之前包含
#include "task.h"
#include "cmsis_os.h"

// HAL 函数声明
void mp_hal_set_interrupt_char(int c);

// 使用 CMSIS-RTOS V2 的时间函数
static inline mp_uint_t mp_hal_ticks_ms(void) {
    return osKernelGetTickCount();  // CMSIS V2 API
}

static inline mp_uint_t mp_hal_ticks_us(void) {
    // 转换为微秒
    return (osKernelGetTickCount() * 1000000) / osKernelGetTickFreq();
}

static inline void mp_hal_delay_ms(mp_uint_t ms) {
    osDelay(ms);  // CMSIS V2 API
}

static inline void mp_hal_delay_us(mp_uint_t us) {
    uint32_t ticks = (us * osKernelGetTickFreq()) / 1000000;
    if (ticks == 0) {
        ticks = 1;
    }
    osDelay(ticks);
}

#endif