/*****************************************************************************/
/***    \file        BatteryStateManagement_proj.c
***     \author      Naveen Balachandar (RBEI/ECK3) 
***
***     \brief       File to monitor the battery condition based required properties
/*****************************************************************************/
/*------ standard includes -------*/
#include <stdio.h>
#include <assert.h>

int batteryTempMonitor_i(float temperature)
{
  if(temperature < 0 || temperature > 45)
  {
    printf("Temperature out of range!\n");
    return 0;
  }
  return 1;
}

int batterySocMonitor_i(float soc) 
{
  if(soc < 20 || soc > 80) 
  {
    printf("State of Charge out of range!\n");
    return 0;
  }
  return 1;
}

int batteryChargerateMonitor_i(float chargeRate)
{
  if(chargeRate > 0.8)
  {
    printf("Charge Rate out of range!\n");
    return 0;
  }
  return 1;
}

int batteryIsOk(float temperature, float soc, float chargeRate)
{ 
 int retTempStat_i, retsocStat_i,retchargeStat_i;
  
  retTempStat_i   = batteryTempMonitor_i(temperature);
  retsocStat_i    = batterySocMonitor_i(soc);
  retchargeStat_i = batteryChargerateMonitor_i(chargeRate);
  
  return ((retTempStat_i|retsocStat_i)|retchargeStat_i);
}

void main() {
  assert(batteryIsOk(25, 70, 0.7));
  assert(!batteryIsOk(50, 85, 0));
}
