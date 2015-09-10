#ifndef X1_h
#define X1_h 1

/* Header file for the code sorting X1 data into something reasonable and sensible. 
 * Author: Nic Hubbard
 * September 2015
 * nicolas.hubbard@york.ac.uk
 * Blatantly copied from W1 Sort
 */

#include <TCutG.h>
#include <vector>
#include <TFile.h>
#include <TTree.h>
#include <vector>
#include <TROOT.h>
#include "multiTDC.h"

void X1SiliconSort(SiliconData* si, float *ADC_import, int ntdc, int *TDC_channel_import, float *TDC_value_import);

bool X1SuppressChannel(int Channel);
double X1EnergyCalc(int Channel, double ADCValue);
bool X1ADCTDCChannelTestPSide(int ADCChannel, int TDCChannel);

void X1Init();

int X1DetHitNumber(int FrontChannel);

int X1StripFront(int FrontChannel);

bool X1TDCFrontTest(int TDCChannel);
int X1TDCIdentifyDetector(int TDCFrontChannel);

#endif
