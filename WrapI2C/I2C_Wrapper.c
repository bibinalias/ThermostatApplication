/*
 * I2C_Wrapper.c
 *
 *  Created on: 18-Apr-2025
 *      Author: Bibin_Alias
 */


#include "stdint.h"
#include "string.h"

#include "sl_udelay.h"
#include "sl_i2cspm.h"
#include "bme68x_defs.h"
#include "I2C_Wrapper.h"


int8_t WRAPI2C_i8Read(uint8_t u8RegAddr, uint8_t *pu8regData, uint32_t u32length,
    void *pvIntfPtr)
    {
  I2C_TransferReturn_TypeDef eReturn_t;
  I2C_TransferSeq_TypeDef sTfrSeq;


  sTfrSeq.addr = (*(uint8_t *)pvIntfPtr) << 1;
  sTfrSeq.flags = I2C_FLAG_WRITE_READ;

  sTfrSeq.buf[0].data = &u8RegAddr;
  sTfrSeq.buf[0].len = 1;
  sTfrSeq.buf[1].data = pu8regData;
  sTfrSeq.buf[1].len = u32length;
  eReturn_t = I2CSPM_Transfer(I2C0, &sTfrSeq);

  return (int8_t)eReturn_t;

    }

int8_t WRAPI2C_i8Write(uint8_t u8RegAddr, const uint8_t *pu8regData, uint32_t u32length,
    void *pvIntfPtr)
    {
  I2C_TransferReturn_TypeDef eReturn_t;
  I2C_TransferSeq_TypeDef sTfrSeq;
  uint8_t au8Buffer[u32length+1];


  sTfrSeq.addr = (*(uint8_t *)pvIntfPtr) << 1;
  sTfrSeq.flags = I2C_FLAG_WRITE;

  au8Buffer[0] = u8RegAddr;
  memcpy(au8Buffer+1, pu8regData, u32length);
  sTfrSeq.buf[0].data = au8Buffer;
  sTfrSeq.buf[0].len = u32length+1;
  sTfrSeq.buf[1].len = 0;
  eReturn_t = I2CSPM_Transfer(I2C0, &sTfrSeq);

  return (int8_t)eReturn_t;
    }
void WRAPI2C_vDelayUs(uint32_t u32Period, void *pvIntfPtr)
{
  (void) pvIntfPtr;

  sl_udelay_wait(u32Period);

}


