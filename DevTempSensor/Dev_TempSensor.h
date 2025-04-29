/*
 * Dev_TempSensor.h
 *
 *  Created on: 18-Apr-2025
 *      Author: Bibin_Alias
 */

#ifndef DEVTEMPSENSOR_DEV_TEMPSENSOR_H_
#define DEVTEMPSENSOR_DEV_TEMPSENSOR_H_

#include "stdbool.h"

bool Dev_bTempSensInit(void);
bool Dev_bGetTemp(float *pfTemp);
bool Dev_bTempSensDeInit(void);
bool Dev_bTempSensSelfTst(void);

#endif /* DEVTEMPSENSOR_DEV_TEMPSENSOR_H_ */
