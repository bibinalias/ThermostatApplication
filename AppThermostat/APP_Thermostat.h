/*
 * APP_Thermostat.h
 *
 *  Created on: 22-Apr-2025
 *      Author: Bibin_Alias
 */

#ifndef APPTHERMOSTAT_APP_THERMOSTAT_H_
#define APPTHERMOSTAT_APP_THERMOSTAT_H_
#include "stdint.h"

#define TEMP_THRESHOLD_HIGH 30.0f
#define TEMP_THRESHOLD_LOW 20.0f
#define MAX_RETRY ((uint8_t)10)
#define LOOP_DELAY_MS ((uint16_t)1000)

void App_vThermostat(void);

#endif /* APPTHERMOSTAT_APP_THERMOSTAT_H_ */
