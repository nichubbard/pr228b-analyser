#ifndef h_LEPSSort
#define h_LEPSSort 1

/* Header file for a LEPS sort
 *
 * Author: Nic Hubbard, September 2015
 * E-mail: nicolas.hubbard@york.ac.uk
 *
 * Let me know if you find mistakes/can suggest corrections
 */

#include "multiTDC.h"
#include "GammaData.h"

void LEPSSort(GammaData* gd, float *ADC_import, int ntdc, int *TDC_channel_import, float *TDC_value_import);

double LEPSEnergyCalc(int Channel, double ADCValue);

int LEPSADCChannelFind(int TDCChannel);

#endif
