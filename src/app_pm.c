#include "tl_common.h"
#include "zcl_include.h"

#include "app_main.h"

#if PM_ENABLE
/**
 *  @brief Definition for wakeup source and level for PM
 */

static drv_pm_pinCfg_t pin_PmCfg[] = {
    {
        BUTTON1,
        PM_WAKEUP_LEVEL
    },
    {
        BUTTON2,
        PM_WAKEUP_LEVEL
    },
};

void app_wakeupPinConfig() {
    drv_pm_wakeupPinConfig(pin_PmCfg, sizeof(pin_PmCfg)/sizeof(drv_pm_pinCfg_t));
}


void app_lowPowerEnter() {

//    drv_pm_wakeupPinLevelChange(pin_PmCfg, sizeof(pin_PmCfg)/sizeof(drv_pm_pinCfg_t));
    drv_pm_lowPowerEnter();
}

int32_t no_joinedCb(void *arg) {

    if (!zb_isDeviceJoinedNwk()) {

        if (tl_stackBusy() || !zb_isTaskDone()) {

//            printf("tl_stackBusy: %s,  zb_isTaskDone: %d\r\n", tl_stackBusy()?"true":"false", zb_isTaskDone());
            return TIMEOUT_1MIN;
        }

#if UART_PRINTF_MODE && DEBUG_PM
        printf("Without network more then 30 minutes! Deep sleep ...\r\n");
#endif

        drv_pm_wakeupPinLevelChange(pin_PmCfg, sizeof(pin_PmCfg)/sizeof(drv_pm_pinCfg_t));

        apsCleanToStopSecondClock();

        drv_disable_irq();
        rf_paShutDown();
        drv_pm_deepSleep_frameCnt_set(ss_outgoingFrameCntGet());

        drv_pm_longSleep(PM_SLEEP_MODE_DEEPSLEEP, PM_WAKEUP_SRC_PAD, 1);
    }

    g_appCtx.timerNoJoinedEvt = NULL;
    return -1;
}


#endif

