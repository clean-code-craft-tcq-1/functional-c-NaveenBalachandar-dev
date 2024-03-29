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
# define GERMAN_LANG_SLECTD 2 
# define ENGLANG_LANG_SLECTD 0

 
/*------ Global variables -------*/
unsigned int langSelected_uint = GERMAN_LANG_SLECTD;/*language selecter*/

const char batPar[6][12] = {"temp","soc","chargerate","temp","Ladezustand","Ladestrom"}; /*Battery par printed for ref lang: German and english*/
const char batLevel[6][12] = {"low","high","Normallevel","niedrig","hoch","Normal"};    /*Battery level printed for ref lang: German and english*/
const char batstatus[4][12] = {"Bad","good","Schlecht","gut"}; /*overall battery status*/


/*Structure type for battery parameter */
 struct BattParmt_str_t
 {
 float batteryParameter;
 float minRange;
 float maxRange;
 float warnLevel;
 int batParIndex;
 int batLevlIndex; 
 int batstat;
 };

static struct BattParmt_str_t *BattParmt_str_p, BattParmt_strPtr_s;

/*---------------------------------------------------------------------------*/
/*     FUNCTION:    batteryCondMonitor_i
 */
/*!    \brief       predecting  the battery states
 * 
 *     \param       current temperature/soc/chargeRate ,respective min and max rate ,index 
 *     \returns     batterystate -low/high/normal
 *
*//*------------------------------------------------------------------------*/
int batteryCondMonitor_i()
{
  if(BattParmt_str_p->batteryParameter < BattParmt_str_p->minRange)   /*Min range valid*/
  {
   printf("Batter parameter %s --> %s!\n",batPar[BattParmt_str_p->batParIndex],batLevel[BattParmt_str_p->batLevlIndex + 0]);
   BattParmt_str_p->batstat = BattParmt_str_p->batLevlIndex + 0;
   return 0;
  }
  else if (BattParmt_str_p->batteryParameter > BattParmt_str_p->maxRange) /*Max range valid*/
  {
   printf("Batter parameter %s -->  %s !\n",batPar[BattParmt_str_p->batParIndex],batLevel[BattParmt_str_p->batLevlIndex + 1]);
   BattParmt_str_p->batstat =  BattParmt_str_p->batLevlIndex + 1;
   return 0;
  }
  else
  {
   printf("Batter parameter %s -->  %s !\n",batPar[BattParmt_str_p->batParIndex],batLevel[BattParmt_str_p->batLevlIndex + 2]);
   BattParmt_str_p->batstat =  BattParmt_str_p->batLevlIndex + 2;
   return 1;
  }
}


/*---------------------------------------------------------------------------*/
/*     FUNCTION:    batteryWarnHandling_v
 */
/*!    \brief       predecting  the warning for max range
 * 
 *     \param       NA 
 *     \returns     NA
 *
*//*------------------------------------------------------------------------*/
void batteryWarnHandling_v()
{
 static float range;
 /*warning level percentage calulcator : 5% of max range*/
  BattParmt_str_p->warnLevel = (0.05 * BattParmt_str_p->maxRange);
  range = (BattParmt_str_p->maxRange - BattParmt_str_p->warnLevel);
 
   /*warning prediction max - Approaching charge-peak*/
    if ((BattParmt_str_p->batteryParameter >= range) && (BattParmt_str_p->batteryParameter < BattParmt_str_p->maxRange) )
    {
     printf("Warn---> %s---->Entering Max\n",batPar[BattParmt_str_p->batParIndex]);
    }
    else
    {
     batteryWarnHandlingMinRange_v();
    }
}


/*---------------------------------------------------------------------------*/
/*     FUNCTION:    batteryWarnHandlingMinRange_v
 */
/*!    \brief       predecting  the warning for min range
 * 
 *     \param       NA 
 *     \returns     NA
 *
*//*------------------------------------------------------------------------*/
void batteryWarnHandlingMinRange_v()
{
 static float range;
 /*warning level percentage calulcator : 5% of max range*/
 
  range = (BattParmt_str_p->minRange +  BattParmt_str_p->warnLevel);
 
   /*warning prediction min - Approaching discharge*/
    if ( (BattParmt_str_p->batteryParameter > BattParmt_str_p->minRange) &&  (BattParmt_str_p->batteryParameter <= range))
    {
     printf("Warn---> %s---->Entering Min\n",batPar[BattParmt_str_p->batParIndex]);
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
 static int overallStat; /*overall status for reporting*/
 static  int conslstat[3]; /*conslidated status for reporting*/
 
  if ( GERMAN_LANG_SLECTD != langSelected_uint)
  {
  /*Validation done with current battery parameter,Min and max range in english lang*/
  BattParmt_str_p->batLevlIndex = 0 ; /*level index for english */ 
  BattParmt_str_p->batteryParameter =temperature;
  BattParmt_str_p->minRange = 0;
  BattParmt_str_p->maxRange =45;
  BattParmt_str_p->batParIndex =0;
  batteryWarnHandling_v();
  retTempStat_i   = batteryCondMonitor_i();
  conslstat[0]= BattParmt_str_p->batstat;
   
  BattParmt_str_p->batteryParameter =soc;
  BattParmt_str_p->minRange = 20;
  BattParmt_str_p->maxRange =80;
  BattParmt_str_p->batParIndex =1;
  batteryWarnHandling_v();
  retsocStat_i    = batteryCondMonitor_i();
  conslstat[1]= BattParmt_str_p->batstat;
   
  BattParmt_str_p->batteryParameter =chargeRate;
  BattParmt_str_p->minRange = 0;
  BattParmt_str_p->maxRange =0.8;
  BattParmt_str_p->batParIndex =2;
  batteryWarnHandling_v(); 
  retchargeStat_i = batteryCondMonitor_i();
  conslstat[2]= BattParmt_str_p->batstat;
   
  }
  else
  {
  /*Validation done with cureent battery parameter,Min and max range in german lang*/
  BattParmt_str_p->batLevlIndex = 3 ; /*level index for english */  
  BattParmt_str_p->batteryParameter =temperature;
  BattParmt_str_p->minRange = 0;
  BattParmt_str_p->maxRange =45;
  BattParmt_str_p->batParIndex =3;
  batteryWarnHandling_v(); 
  retTempStat_i   = batteryCondMonitor_i();
  conslstat[0]= BattParmt_str_p->batstat;
   
  BattParmt_str_p->batteryParameter =soc;
  BattParmt_str_p->minRange = 20;
  BattParmt_str_p->maxRange =80;
  BattParmt_str_p->batParIndex =4;
  batteryWarnHandling_v();
  retsocStat_i    = batteryCondMonitor_i();
  conslstat[1]= BattParmt_str_p->batstat;
   
  BattParmt_str_p->batteryParameter =chargeRate;
  BattParmt_str_p->minRange = 0;
  BattParmt_str_p->maxRange =0.8;
  BattParmt_str_p->batParIndex =5; 
  batteryWarnHandling_v(); 
  retchargeStat_i = batteryCondMonitor_i();
  conslstat[2]= BattParmt_str_p->batstat;
   
  }
 overallStat = ((retTempStat_i & retsocStat_i) & retchargeStat_i);
  
  /*reporting conslidated and overall status to Controller X to take necessary action*/
  battecondreportControllerX_v(&overallStat ,conslstat);
  /*return battery state ok /nok*/
  return overallStat ;
 
}

/*---------------------------------------------------------------------------*/
/*     FUNCTION:    battecondreportControllerX_v
 */
/*!    \brief       reporting conslidated and overall status to Controller X to take necessary action
 * 
 *     \param       NA 
 *     \returns     NA
 *
*//*------------------------------------------------------------------------*/
void battecondreportControllerX_v(int *overallStat ,int conslstat[])
{
 /*overall bat status*/
 int Batstatrxvd = *overallStat;
 /*consolidated status*/
 int tempstat = conslstat[0];
 int socstat = conslstat[1];
 int chargestat = conslstat[2];
 
 /*overall status of battery*/
 printf("Contrller X :combined parameter status  -->  %s !\n",batstatus[Batstatrxvd + langSelected_uint]);
 
 /*Consolidated status of battery*/
 printf("Contrller X :Consolidated status  --> temp: %s SOC:%s Chargerate:%s\n",batLevel[tempstat],batLevel[socstat],batLevel[chargestat]); 

}

int main() 
{
 /*Initilizing the ptr */
  BattParmt_str_p = &BattParmt_strPtr_s;
 
  /*Unit test cases*/
  /*case 1 : all conditons valid*/
  assert(batteryStateValidation_i(25, 70, 0.7));
  /*case 2 : charge rate dec case */
   assert(batteryStateValidation_i(29, 33, 0.03));
  /*case 3 : charge rate inc case */
   assert(batteryStateValidation_i(30, 35, 0.77));
  /*case 4 : charge rate alone valid*/
  assert(!batteryStateValidation_i(50, 85, 0));
  /*case 5 :boundary check maximum supported range*/
   assert(batteryStateValidation_i(44, 79, 0.7));
  /*case 6 :boundary check  minimum supported range*/
  assert(batteryStateValidation_i(0.1, 21, 0));
  /*case 7 :boundary check  maximum ->out of range all parameter invalid*/
   assert(!batteryStateValidation_i(46, 81, 0.9));
  /*case 8 : temp alone valid*->should fail*/
   assert(!batteryStateValidation_i(30, 85, 0.9));
   /*case 9 : soc alone valid*-> should fail */
   assert(!batteryStateValidation_i(60, 72, 0.9));
   /*case 10 : soc min range*-> should fail */
   assert(!batteryStateValidation_i(30, 18, 0.6));
   return 0;
}
