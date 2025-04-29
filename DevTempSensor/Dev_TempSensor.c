/*
 * Dev_TempSensor.c
 *
 *  Created on: 18-Apr-2025
 *      Author: Bibin_Alias
 */
#include "stdbool.h"
#include "stdio.h"

#include "Dev_TempSensor.h"
#include "I2C_Wrapper.h"
#include "bme68x_defs.h"
#include "bme68x.h"


static  struct bme68x_dev sBme680Dev;
static   uint32_t u32MesDur;
uint8_t i8TempSensorAddr = BME68X_I2C_ADDR_LOW;
bool Dev_bTempSensInit(void)
{
  int8_t i8Return;



  struct bme68x_conf sBme680Conf;

  sBme680Dev.intf = BME68X_I2C_INTF;
  sBme680Dev.intf_ptr = &i8TempSensorAddr;
  sBme680Dev.read = WRAPI2C_i8Read;
  sBme680Dev.write = WRAPI2C_i8Write;
  sBme680Dev.delay_us = WRAPI2C_vDelayUs;

  do
    {
      i8Return = bme68x_init(&sBme680Dev);
      if(i8Return != BME68X_OK)
        {
          printf("Failed\t:\tbme68x_init()");
          printf("Error Code : %d\n",i8Return);
          break;

        }
      else
        {
          printf("Passed\t:\tbme68x_init()\n");
        }



      sBme680Conf.filter = BME68X_FILTER_SIZE_3;
      sBme680Conf.os_temp = BME68X_OS_8X;
      sBme680Conf.os_hum = sBme680Conf.os_pres = BME68X_OS_NONE;
      sBme680Conf.odr = BME68X_ODR_NONE;

      i8Return = bme68x_set_conf(&sBme680Conf, &sBme680Dev);
      if(i8Return != BME68X_OK)
        {
          printf("Failed\t:\tbme68x_set_conf()\n");
          printf("Error Code : %d\n",i8Return);
          break;

        }
      else
        printf("Passed\t:\tbme68x_set_conf()\n");

      u32MesDur = bme68x_get_meas_dur(BME68X_FORCED_MODE, &sBme680Conf, &sBme680Dev);
      printf("Measurement Duration \t:\t%ld uS\n",u32MesDur);

    }while(0);


  return !i8Return;
}

bool Dev_bGetTemp(float *pfTemp)
{
  int8_t i8Return;
  struct bme68x_data sBme680Data;
  uint8_t u8NData;

  do
    {
      i8Return = bme68x_set_op_mode(BME68X_FORCED_MODE, &sBme680Dev);
      if(i8Return != BME68X_OK)
        {
          printf("Failed\t:\tbme68x_set_op_mode()\n");
          printf("Error Code : %d\n",i8Return);
          break;

        }
/*      else
        printf("Passed\t:\tbme68x_set_op_mode()\n");*/

      i8Return = bme68x_get_data(BME68X_FORCED_MODE, &sBme680Data, &u8NData,&sBme680Dev);
      if(i8Return != BME68X_OK)
        {
          printf("Failed\t:\tbme68x_get_data()\n");
          printf("Error Code : %d\n",i8Return);
          break;

        }
    /*  else
        printf("Passed\t:\tbme68x_get_data()\n");*/

     // printf("n_data : %d\n Data_status :%02X\n",u8NData,sBme680Data.status);
      if(u8NData >= 1 && (sBme680Data.status & BME68X_NEW_DATA_MSK))
        *pfTemp = sBme680Data.temperature;
      else
        {
          i8Return = BME68X_W_NO_NEW_DATA;
          printf("Failed\t:\tNew data NOT available\n");
          printf("Error Code : %d\n",i8Return);
        }


    }while(0);

  return !i8Return;
}

bool Dev_bTempSensDeInit(void)
{

  int8_t i8Return;

  i8Return =bme68x_soft_reset(&sBme680Dev);
  if(i8Return != BME68X_OK)
     {
       printf("Failed\t:\tbme68x_soft_reset(\n");
       printf("Error Code : %d\n",i8Return);

     }
   else
     printf("Passed\t:\tbme68x_soft_reset()\n");
  return !i8Return;

}

bool Dev_bTempSensSelfTst(void)// need to perform before Init
{
  printf("Started\t:\tbme68x_selftest_check()\n");
  int8_t i8Return;
  struct bme68x_dev sTstBme680Dev;
  sTstBme680Dev.intf = BME68X_I2C_INTF;
  sTstBme680Dev.intf_ptr = &i8TempSensorAddr;
  sTstBme680Dev.read = WRAPI2C_i8Read;
  sTstBme680Dev.write = WRAPI2C_i8Write;
  sTstBme680Dev.delay_us = WRAPI2C_vDelayUs;
  i8Return = bme68x_selftest_check(&sTstBme680Dev);
  if(i8Return != BME68X_OK)
    {
      printf("Failed\t:\tbme68x_selftest_check(\n");
      printf("Error Code : %d\n",i8Return);

    }
  else
    printf("Passed\t:\tbme68x_selftest_check()\n");
  return !i8Return;
}
