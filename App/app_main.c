#include "app_main.h"

#include "stdio.h"
#include "cmsis_os.h"

int app_main(void)
{
    while (1) {
        printf("Hello World\r\n");
        osDelay(1000);
    }
    return 0;
} 