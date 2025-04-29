/*
 * APP_Thermostat.c
 *
 *  Created on: 22-Apr-2025
 *      Author: Bibin_Alias
 */

#include "stdbool.h"
#include "stdio.h"
#include "stdint.h"
#include "sl_udelay.h"

#include "APP_Thermostat.h"
#include "Dev_Pump.h"
#include "Dev_TempSensor.h"


static bool App_vThermostatLoop(void)
{
  float fTemp,fPrevTemp = -300;
  char cTempStatus;
  bool bReturn;
  uint8_t u8Retry = 0;

  while(1)
    {
      fPrevTemp = fTemp;
      bReturn = Dev_bGetTemp(&fTemp);
      if(!bReturn)
        {
          u8Retry++;
          printf("Failed\t:\tDev_bGetTemp()\tRetry Count :%d\n",u8Retry);
          if(u8Retry > MAX_RETRY)
            {
              printf("Failed\t:\t Retry Count > %d",MAX_RETRY);
              break;
            }
          else
            {
              continue;
            }

        }
      switch(ePumpStatus)
      {
        case PUMP_OFF:
          if(fTemp <= TEMP_THRESHOLD_LOW)
            {
              bReturn = Dev_bPumpOn();
              if(!bReturn)
                {
                  u8Retry++;
                  printf("Failed\t:\tDev_bPumpOn()\tRetry Count :%d\n",u8Retry);
                  if(u8Retry > MAX_RETRY)
                    {
                      printf("Failed\t:\t Retry Count > %d",MAX_RETRY);
                      break;
                    }
                  else
                    {
                      continue;
                    }

                }
              printf("PUMP ON\t\t");
              printf("Temperature : %.2f\xB0%C\t%C\n",fTemp,'C',cTempStatus);
            }

          break;
        case PUMP_ON:
          if(fTemp >= TEMP_THRESHOLD_HIGH)
            {
              bReturn = Dev_bPumpOff();
              if(!bReturn)
                {
                  u8Retry++;
                  printf("Failed\t:\tDev_bPumpOff()\tRetry Count :%d\n",u8Retry);
                  if(u8Retry > MAX_RETRY)
                    {
                      printf("Failed\t:\t Retry Count > %d",MAX_RETRY);
                      break;
                    }
                  else
                    {
                      continue;
                    }

                }
              printf("PUMP OFF\t\t");
              printf("Temperature : %.2f\xB0%C\n",fTemp,'C');
            }
          break;
        default:
          printf("Failed\t:\tLogic Not Implemented\n");
      }

      if(fPrevTemp < -273)
        cTempStatus = 'x';
      else if(fTemp > fPrevTemp)
        cTempStatus = '^';
      else if(fTemp < fPrevTemp)
        cTempStatus = 'v';
      else
        cTempStatus = '=';
      fPrevTemp = fTemp;
      printf("Temperature : %.2f\xB0%c %c\n",fTemp,'C',cTempStatus);

      u8Retry =0;
      sl_udelay_wait(LOOP_DELAY_MS * 1000);
      fPrevTemp = fTemp;

    }
  return bReturn;

}
void App_vThermostat(void)
{
  if(Dev_bPump_init())
    if(Dev_bPumpOff())
      if(Dev_bTempSensSelfTst())
        if(Dev_bTempSensInit())
          {
            App_vThermostatLoop();
            Dev_bPumpOff();
            Dev_bTempSensDeInit();
          }

  printf("Error\t:\t function App_vThermostat exited due to some error\n");
}
