/*
 * Dev_Pump.h
 *
 *  Created on: 17-Apr-2025
 *      Author: Bibin_Alias
 */

#ifndef DEVPUMP_DEV_PUMP_H_
#define DEVPUMP_DEV_PUMP_H_

#include "stdbool.h"

typedef enum{
  PUMP_OFF,
  PUMP_ON,
}eDev_PumpStatus_t;

extern eDev_PumpStatus_t ePumpStatus;
bool Dev_bPump_init(void);
bool Dev_bPumpOn(void);
bool Dev_bPumpOff(void);
#endif /* DEVPUMP_DEV_PUMP_H_ */
