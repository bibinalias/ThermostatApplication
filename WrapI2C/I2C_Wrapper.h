/*
 * I2C_Wrapper.h
 *
 *  Created on: 18-Apr-2025
 *      Author: Bibin_Alias
 */

#ifndef DEV_TEMPSENSOR_I2C_WRAPPER_H_
#define DEV_TEMPSENSOR_I2C_WRAPPER_H_
#include "stdint.h"


void WRAPI2C_vDelayUs(uint32_t u32Period, void *pvIntfPtr);
int8_t WRAPI2C_i8Write(uint8_t u8RegAddr, const uint8_t *pu8regData, uint32_t u32length,
    void *pvIntfPtr);
int8_t WRAPI2C_i8Read(uint8_t u8RegAddr, uint8_t *pu8regData, uint32_t u32length,
    void *pvIntfPtr);




#endif /* DEV_TEMPSENSOR_I2C_WRAPPER_H_ */
