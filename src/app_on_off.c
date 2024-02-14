#include "tl_common.h"
#include "zcl_include.h"

#include "app_main.h"

//static uint8_t toggle = false;

void cmdOnOff(uint8_t endpoint) {
    epInfo_t dstEpInfo;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);

    dstEpInfo.profileId = HA_PROFILE_ID;
#if FIND_AND_BIND_SUPPORT
        dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;
#else
    dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
    dstEpInfo.dstEp = endpoint;
    dstEpInfo.dstAddr.shortAddr = 0xfffc;
#endif

    zcl_onOff_toggleCmd(endpoint, &dstEpInfo, FALSE);

//    if (!toggle) {
//        zcl_onOff_onCmd(endpoint, &dstEpInfo, FALSE);
//        toggle = true;
//    } else {
//        zcl_onOff_offCmd(endpoint, &dstEpInfo, FALSE);
//        toggle = false;
//    }
}
