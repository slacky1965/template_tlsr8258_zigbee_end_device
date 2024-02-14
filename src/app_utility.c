#include "tl_common.h"
#include "zcl_include.h"

#include "app_main.h"

uint8_t mcuBootAddrGet(void);

void start_message() {
#ifdef ZCL_OTA
    if (mcuBootAddrGet()) {
#if UART_PRINTF_MODE
        printf("OTA mode enabled. MCU boot from address: 0x%x\r\n", FLASH_ADDR_OF_OTA_IMAGE);
#endif /* UART_PRINTF_MODE */
    } else {
#if UART_PRINTF_MODE
        printf("OTA mode enabled. MCU boot from address: 0x%x\r\n", FLASH_ADDR_OF_APP_FW);
#endif /* UART_PRINTF_MODE */
    }
#else
#if UART_PRINTF_MODE
    printf("OTA mode desabled. MCU boot from address: 0x%x\r\n", FLASH_ADDR_OF_APP_FW);
#endif /* UART_PRINTF_MODE */

#endif
}


int32_t poll_rateAppCb(void *arg) {

    uint32_t poll_rate = zb_getPollRate();

    if (poll_rate == g_appCtx.long_poll) {
        zb_setPollRate(g_appCtx.short_poll);
        return TIMEOUT_30SEC;
    }

    zb_setPollRate(g_appCtx.long_poll);

    return g_appCtx.long_poll;
}

int32_t delayedMcuResetCb(void *arg) {

    //printf("mcu reset\r\n");
    zb_resetDevice();
    return -1;
}

int32_t delayedFactoryResetCb(void *arg) {

    //printf("factory reset\r\n");
    zb_resetDevice2FN();
//    zb_factoryReset();
//    //sleep_ms(500);
//    zb_resetDevice();
    return -1;
}

int32_t delayedFullResetCb(void *arg) {

    //printf("full reset\r\n");
    return -1;
}
