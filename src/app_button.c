#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "app_main.h"

static void buttonKeepPressed(u8 btNum) {
    g_appCtx.button[btNum-1].state = APP_FACTORY_NEW_DOING;
    g_appCtx.button[btNum-1].ctn = 0;

    if(btNum == VK_SW1) {
        printf("VK_SW1. 5 sec. Factory reset\r\n");
        delayedFactoryResetCb(NULL);
//        zb_factoryReset();
    }else if(btNum == VK_SW2){
        printf("VK_SW2. 5 sec\r\n");
    }
}


static void buttonSinglePressed(u8 btNum) {

    printf("Command single click\r\n");

    switch (btNum) {
        case VK_SW1:
            printf("Single pressed SW1\r\n");
            if(zb_isDeviceJoinedNwk()){
                cmdOnOff(APP_ENDPOINT1);
            }
            break;
        case VK_SW2:
            printf("Single pressed SW2\r\n");
            if(zb_isDeviceJoinedNwk()){
            }
            break;
        default:
            break;
    }
}

static void buttonDoublePressed(u8 btNum) {
    printf("Command double click\r\n");
}

static void buttonTriplePressed(u8 btNum) {
    printf("Command triple click\r\n");
}

static void buttonQuadruplePressed(u8 btNum) {
    printf("Command quadruple click\r\n");
}


static void buttonCheckCommand(uint8_t btNum) {
    g_appCtx.button[btNum-1].state = APP_STATE_NORMAL;

    if (g_appCtx.button[btNum-1].ctn == 1) {
        buttonSinglePressed(btNum);
    } else if (g_appCtx.button[btNum-1].ctn == 2) {
        buttonDoublePressed(btNum);
    } else if (g_appCtx.button[btNum-1].ctn == 3) {
        buttonTriplePressed(btNum);
    } else if (g_appCtx.button[btNum-1].ctn == 4) {
        buttonQuadruplePressed(btNum);
    }

    g_appCtx.button[btNum-1].ctn = 0;
}


void keyScan_keyPressedCB(kb_data_t *kbEvt) {
    //u8 toNormal = 0;
    u8 keyCode = kbEvt->keycode[0];
    //static u8 lastKeyCode = 0xff;

//    buttonShortPressed(keyCode);


    if(keyCode != 0xff) {
        g_appCtx.button[keyCode-1].pressed_time = clock_time();
        g_appCtx.button[keyCode-1].state = APP_FACTORY_NEW_SET_CHECK;
        g_appCtx.button[keyCode-1].ctn++;
    }
}


void keyScan_keyReleasedCB(u8 keyCode){
    if (keyCode != 0xff) {
        g_appCtx.button[keyCode-1].released_time = clock_time();
        g_appCtx.button[keyCode-1].state = APP_STATE_RELEASE;

//        g_appCtx.button[keyCode-1].state = APP_STATE_NORMAL;
    }
}

void button_handler(void) {
    static u8 valid_keyCode = 0xff;

    for (uint8_t i = 0; i < MAX_BUTTON_NUM; i++) {
        if (g_appCtx.button[i].state == APP_FACTORY_NEW_SET_CHECK) {
            if(clock_time_exceed(g_appCtx.button[i].pressed_time, TIMEOUT_TICK_5SEC)) {
                buttonKeepPressed(i+1);
            }
        }

        if (g_appCtx.button[i].state == APP_STATE_RELEASE) {
            if(clock_time_exceed(g_appCtx.button[i].released_time, TIMEOUT_TICK_250MS)) {
                buttonCheckCommand(i+1);
            }

        }
    }

    if(kb_scan_key(0, 1)){
        if(kb_event.cnt){
            g_appCtx.keyPressed = 1;
            keyScan_keyPressedCB(&kb_event);
            if(kb_event.cnt == 1){
                valid_keyCode = kb_event.keycode[0];
            }
        }else{
            keyScan_keyReleasedCB(valid_keyCode);
            valid_keyCode = 0xff;
            g_appCtx.keyPressed = 0;
        }
    }
}

u8 button_idle() {

    if (g_appCtx.keyPressed) {
        return true;
    }

    for (uint8_t i = 0; i < MAX_BUTTON_NUM; i++) {
        if (g_appCtx.button[i].ctn) return true;
    }

    return false;
}
