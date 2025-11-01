#include "mphalport.h"
#include "main.h"  // CubeMX 生成的头文件
#include <stdio.h>  // 需要 sprintf

// 假设你的 UART 句柄是 huart1
extern UART_HandleTypeDef huart1;

// 接收字符（阻塞）
int mp_hal_stdin_rx_chr(void) {
    uint8_t c = 0;
    // 使用 HAL_MAX_DELAY 阻塞等待
    HAL_UART_Receive(&huart1, &c, 1, HAL_MAX_DELAY);
    return c;
}

// 发送字符串
void mp_hal_stdout_tx_strn(const char *str, size_t len) {
    HAL_UART_Transmit(&huart1, (uint8_t*)str, len, HAL_MAX_DELAY);
}

// 中断字符设置
static int mp_interrupt_char = -1;

void mp_hal_set_interrupt_char(int c) {
    mp_interrupt_char = c;
}