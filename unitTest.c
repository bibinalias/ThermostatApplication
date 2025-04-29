/*
 * unitTest.c
 *
 *  Created on: 17-Apr-2025
 *      Author: Bibin_Alias
 */

#include "bme68x.h"
#include "sl_i2cspm.h"
#include "em_i2c.h"
#include "stdio.h"
#include "string.h"
#include "APP_Thermostat.h"
#include "Dev_TempSensor.h"
#include "I2C_Wrapper.h"

void tempsensorTest(void)
{
  App_vThermostat();

}

void tempsensorTestxa(void)
{
  //vUnitTestI2cCom();
  float fTemp;
  bool bReturn;

  bReturn = Dev_bTempSensSelfTst();
  printf("Dev_bTempSensSelfTst Result = %d\n",bReturn);
  bReturn = Dev_bTempSensInit();
  printf("Dev_bTempSensInit Result = %d\n",bReturn);
  do
    {
      bReturn = Dev_bGetTemp(&fTemp);
      printf("Dev_bGetTemp Result = %d\n",bReturn);
      printf("fTemp = %f\n",fTemp);


    }while(1);



  bReturn = Dev_bTempSensDeInit();
  printf("Dev_bTempSensDeInit Result = %d\n",bReturn);

}


int8_t i2c_read_hw(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len) {
  I2C_TransferSeq_TypeDef seq;
  seq.addr = dev_addr << 1;
  seq.flags = I2C_FLAG_WRITE_READ;

  uint8_t write_buf[1] = { reg_addr };
  seq.buf[0].data = write_buf;
  seq.buf[0].len = 1;
  seq.buf[1].data = data;
  seq.buf[1].len = len;

  I2C_TransferReturn_TypeDef result = I2CSPM_Transfer(I2C0, &seq);
  return (result == i2cTransferDone) ? 0 : -1;
}

int8_t i2c_write_hw(uint8_t dev_addr, uint8_t reg_addr, const uint8_t *data, uint16_t len) {
  I2C_TransferSeq_TypeDef seq;
  uint8_t buffer[1 + len];

  buffer[0] = reg_addr;
  memcpy(&buffer[1], data, len);

  seq.addr = dev_addr << 1;
  seq.flags = I2C_FLAG_WRITE;
  seq.buf[0].data = buffer;
  seq.buf[0].len = 1 + len;
  seq.buf[1].len = 0;

  I2C_TransferReturn_TypeDef result = I2CSPM_Transfer(I2C0, &seq);
  return (result == i2cTransferDone) ? 0 : -1;
}

void delay_ms_hw(uint32_t ms) {
  for (volatile uint32_t i = 0; i < ms * 4000; i++);
}

int8_t user_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr) {
  uint8_t dev_addr = *(uint8_t *)intf_ptr;
  return i2c_read_hw(dev_addr, reg_addr, reg_data, (uint16_t)len);
}

int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr) {
  uint8_t dev_addr = *(uint8_t *)intf_ptr;
  return i2c_write_hw(dev_addr, reg_addr, reg_data, (uint16_t)len);
}

void user_delay_us(uint32_t period, void *intf_ptr) {
  (void)intf_ptr;
  delay_ms_hw(period / 1000);
}
#define BME680_I2C_ADDR 0x76    // or 0x77 depending on your sensor's ADDR pin
#define BME680_CHIP_ID_REG 0xD0

/**
 * Read a register from BME680 over I2C using I2CSPM HAL
 */
int bme680_i2c_test(void) {
  uint8_t chip_id = 0;

  // Prepare I2C transfer sequence
  I2C_TransferSeq_TypeDef seq;
  seq.addr = BME680_I2C_ADDR << 1;
  seq.flags = I2C_FLAG_WRITE_READ;

  uint8_t write_buf[1] = { BME680_CHIP_ID_REG };
  seq.buf[0].data = write_buf;
  seq.buf[0].len = 1;

  seq.buf[1].data = &chip_id;
  seq.buf[1].len = 1;

  // Perform I2C transfer
  I2C_TransferReturn_TypeDef result = I2CSPM_Transfer(I2C0, &seq);

  if (result == i2cTransferDone) {
      if (chip_id == 0x61) {
          printf("BME680 detected!\n Who am I ? \n I'm  0X%02X\n", chip_id);
          return 0;
      } else {
          printf("Unexpected Chip ID: 0x%02X\n", chip_id);
          return -2;
      }
  } else {
      printf("I2C transfer failed: %d\n", result);
      return -1;
  }
}

int tempsensorTestx(void) {


  struct bme68x_dev gas_sensor;
  struct bme68x_conf conf;
  struct bme68x_heatr_conf heatr_conf;
  struct bme68x_data data;
  uint8_t dev_addr = BME68X_I2C_ADDR_LOW; // 0x76
  int8_t rslt;
  uint8_t n_fields;

  // Set up driver interface
  gas_sensor.intf = BME68X_I2C_INTF;
  gas_sensor.read = WRAPI2C_i8Read;
  gas_sensor.write = WRAPI2C_i8Write;
  gas_sensor.delay_us = WRAPI2C_vDelayUs;
  gas_sensor.intf_ptr = &dev_addr;
  gas_sensor.amb_temp = 25;

  // Initialize sensor
  rslt = bme68x_init(&gas_sensor);
  if (rslt != BME68X_OK) {
      printf("Init failed\n");
      return -1;
  }

  // Sensor configuration
  conf.filter = BME68X_FILTER_OFF;
  conf.odr = BME68X_ODR_NONE;
  conf.os_hum = BME68X_OS_2X;
  conf.os_pres = BME68X_OS_4X;
  conf.os_temp = BME68X_OS_8X;
  rslt = bme68x_set_conf(&conf, &gas_sensor);

  // Heater config (for gas measurement)
  heatr_conf.enable = BME68X_ENABLE;
  heatr_conf.heatr_temp = 320;
  heatr_conf.heatr_dur = 150;
  rslt = bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatr_conf, &gas_sensor);



  while (1)
    {
      gas_sensor.delay_us(200000, NULL);
      // Set operation mode
      rslt = bme68x_set_op_mode(BME68X_FORCED_MODE, &gas_sensor);
      // Wait for measurement
      gas_sensor.delay_us(200000, NULL);

      // Read data
      rslt = bme68x_get_data(BME68X_FORCED_MODE, &data, &n_fields, &gas_sensor);

      if (rslt == BME68X_OK && n_fields > 0) {
          printf("Temp: %f °C\n", data.temperature);
          printf("Humidity: %f %%\n", data.humidity);
          printf("Pressure: %.f hPa\n", data.pressure / 100.0);
          printf("Gas Resistance: %f ohms\n", data.gas_resistance);
      } else {
          printf("No data or error: %d\n", rslt);
      }
    }


}



void vUnitTestI2cCom(void)
{
  uint8_t u8Return;
  uint8_t u8Intf = 0x76;
  uint8_t u8Gaswait0Addr = 0x64;
  uint8_t u8Gaswait0Data;
  printf("Start Testing I2C Wrapper\n\n");
  printf("Waiting 2 seconds\n");
  WRAPI2C_vDelayUs(1000000,&u8Intf);
  printf("1 S...\n");
  WRAPI2C_vDelayUs(1000000,&u8Intf);
  printf("2 S...\n\n");
  printf("Reading Gas wait 0 register\n");
  u8Return = WRAPI2C_i8Read(u8Gaswait0Addr,&u8Gaswait0Data,1 ,&u8Intf);
  if(u8Return != 0)
    printf("WRAPI2C_i8Read failed and returned %d\n",u8Return);
  else
    printf("Gas Wait 0 Register value is 0X%02X\n",u8Gaswait0Data);

  u8Gaswait0Data = 0xBA;
  printf("Writing 0X%02X to Gas wait 0 register\n",u8Gaswait0Data);

  u8Return = WRAPI2C_i8Write(u8Gaswait0Addr, &u8Gaswait0Data, 1, &u8Intf);
  if(u8Return != 0)
    printf("WRAPI2C_i8Read failed and returned %d\n",u8Return);
  else
    printf("Gas Wait 0 Register value changed\n");
  printf("Reading Gas wait 0 register\n");
  u8Return = WRAPI2C_i8Read(u8Gaswait0Addr,&u8Gaswait0Data,1 ,&u8Intf);
  if(u8Return != 0)
    printf("WRAPI2C_i8Read failed and returned %d\n",u8Return);
  else
    printf("Gas Wait 0 Register value is 0X%02X\n",u8Gaswait0Data);

  if(u8Gaswait0Data == 0xBA)
    printf("All I2C wrapper functions are Correct only\n\n\n");
}
