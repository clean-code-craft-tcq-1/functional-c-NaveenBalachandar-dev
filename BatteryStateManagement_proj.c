/*****************************************************************************/
/***    \file        BatteryStateManagement_proj.c
***     \author      Naveen Balachandar (RBEI/ECK3) 
***
***     \brief       File to monitor the battery condition based required properties
/*****************************************************************************/
/*------ standard includes -------*/
#include <stdio.h>
#include <assert.h>

/*Macros*/
#define GERMANLANG 1;
#define ENGLANG 0;

/*------ Global variables -------*/
uint8  langSelected_uint8 = ENGLANG ;/*default english language*/
char batPar[3][10] = {"temp","soc","chargerate"}; /*Battery par printed for ref*/

/*---------------------------------------------------------------------------*/
/*     FUNCTION:    batteryCondMonitor_i
 */
/*!    \brief       predecting  the battery states
 * 
 *     \param       current temperature/soc/chargeRate ,respective min and max rate ,index 
 *     \returns     batterystate -low/high/normal
 *
*//*------------------------------------------------------------------------*/
int batteryCondMonitor_i(float batteryParameter ,float minRange, float maxRange,int batParIndex)
{
 
  if (langSelected_uint8 == ENGLANG)
  {
  if(batteryParameter < minRange)   /*Min range valid*/
  {
   printf("Battery parameter %s is low!\n",batPar[batParIndex]);
   return 0;
  }
  else if (batteryParameter > maxRange) /*Max range valid*/
  {
   printf("Battery parameter %s is High !\n",batPar[batParIndex]);
   return 0;
  }
  else
  {
   printf("Battery parameter %s is Normal !\n",batPar[batParIndex]);
   return 1;
  }
  }
  else
  {
   printf("Battery parameter %s is Normal !\n",batPar[batParIndex]);
   return 1;
    
  }
}

/*---------------------------------------------------------------------------*/
/*     FUNCTION:    batteryStateValidation_i
 */
/*!    \brief       Monitoring the battery states
 * 
 *     \param       temperature,soc,chargeRate
 *     \returns     batterystate -OK/NOK
 *
*//*------------------------------------------------------------------------*/
int batteryStateValidation_i(float temperature, float soc, float chargeRate)
{ 
 
 int retTempStat_i, retsocStat_i,retchargeStat_i;
  
  /*Validation done with cureent battery parameter,Min and max range*/
  retTempStat_i   = batteryCondMonitor_i(temperature,0,45,0);
  retsocStat_i    = batteryCondMonitor_i(soc,20,80,1);
  retchargeStat_i = batteryCondMonitor_i(chargeRate,0,0.8,2);
  
  /*return battery state ok /nok*/
  return ((retTempStat_i & retsocStat_i) & retchargeStat_i);
}

void main() 
{
  /*Unit test cases*/
  /*case 1 : all conditons valid*/
  assert(batteryStateValidation_i(25, 70, 0.7));
  /*case 2 : charge rate alone valid*/
  assert(!batteryStateValidation_i(50, 85, 0));
  /*case 3 :boundary check maximum supported range*/
   assert(batteryStateValidation_i(44, 79, 0.7));
  /*case 4 :boundary check  minimum supported range*/
  assert(batteryStateValidation_i(0.1, 21, 0));
  /*case 5 :boundary check  maximum ->out of range all parameter invalid*/
   assert(!batteryStateValidation_i(46, 81, 0.9));
  /*case 6 : temp alone valid*->should fail*/
   assert(!batteryStateValidation_i(30, 85, 0.9));
   /*case 7 : soc alone valid*-> should fail */
   assert(!batteryStateValidation_i(60, 72, 0.9));
 
}
