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
# define GERMAN_LANG_SLECTD 1
# define ENGLANG_LANG_SLECTD 0
 
/*------ Global variables -------*/
unsigned int langSelected_uint = GERMAN_LANG_SLECTD;/*language selecter*/
char batPar[6][12] = {"temp","soc","chargerate","temp","Ladezustand","Ladestrom"}; /*Battery par printed for ref lang: German and english*/
char batLevel[6][12] = {"low","high","Normallevel","niedrig","hoch","Normal"};    /*Battery level printed for ref lang: German and english*/

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
 
  if(batteryParameter < minRange)   /*Min range valid*/
  {
   printf("Batter parameter %s --> %s!\n",batPar[batParIndex],batLevel[batParIndex]);
   return 0;
  }
  else if (batteryParameter > maxRange) /*Max range valid*/
  {
   printf("Batter parameter %s -->  %s !\n",batPar[batParIndex],batLevel[batParIndex]);
   return 0;
  }
  else
  {
   printf("Batter parameter %s -->  %s !\n",batPar[batParIndex],batLevel[batParIndex]);
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
 
  if ( GERMAN_LANG_SLECTD != langSelected_uint)
  {
  /*Validation done with current battery parameter,Min and max range in english lang*/
  retTempStat_i   = batteryCondMonitor_i(temperature,0,45,0);
  retsocStat_i    = batteryCondMonitor_i(soc,20,80,1);
  retchargeStat_i = batteryCondMonitor_i(chargeRate,0,0.8,2);
  }
  else
  {
  /*Validation done with cureent battery parameter,Min and max range in german lang*/
  retTempStat_i   = batteryCondMonitor_i(temperature,0,45,3);
  retsocStat_i    = batteryCondMonitor_i(soc,20,80,4);
  retchargeStat_i = batteryCondMonitor_i(chargeRate,0,0.8,5);
  }
  
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
