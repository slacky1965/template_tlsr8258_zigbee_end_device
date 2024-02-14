#ifndef SRC_INCLUDE_APP_PM_H_
#define SRC_INCLUDE_APP_PM_H_

#define CHK_DEEP_SLEEP       DEEP_ANA_REG1

#if PM_ENABLE

void app_wakeupPinConfig();
void app_lowPowerEnter();
int32_t no_joinedCb(void *arg);

#endif

#endif /* SRC_INCLUDE_APP_PM_H_ */
