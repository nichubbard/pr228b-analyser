/*
 * Sort code for the X1s in conjunction with the K600 spectrometer
 * This code 'plugs in' to f-plane.c and is called from there.
 * It populates the SiliconData class in that code and chucks the data out into the ROOT tree 'DATA'.
 *
 * Date: September 2015
 *
 */

#include "SiliconData.h"
#include "X1.h"

extern int ADCModules;
extern int ADCsize;
extern int NumberOfX1;

extern int **X1ADCChannelLimits;
extern int **X1TDCChannelLimits;

extern double *ADCOffsets;
extern double *ADCGains;
extern int *ADCPedestals;
extern double *TDCOffsets;

void X1SiliconSort(SiliconData* si, float *ADC_import, int ntdc,
    int *TDC_channel_import, float *TDC_value_import)
{
  multiTDC mTDC;
  mTDC.multiTDCSort(ntdc, TDC_channel_import, TDC_value_import);

  for (int k = 0; k < mTDC.GetSize(); k++)
  {
    int tdcFront = mTDC.GetChannel(k);
    if (X1TDCFrontTest(tdcFront))
    {
      int DetNum = X1TDCIdentifyDetector(tdcFront);
      if (DetNum > 0 && DetNum != 3)
      {
        for (int i = X1ADCChannelLimits[DetNum][0]; i <= X1ADCChannelLimits[DetNum][1]; i++)
        {
          if (X1ADCTDCChannelTestPSide(i, tdcFront) && ADC_import[i] > ADCPedestals[i])
          {
            double energyi = X1EnergyCalc(i,ADC_import[i]);
            si->SetEnergy(energyi);
            si->SetTheta(-1);
            si->SetPhi(-1);
            si->SetTime(mTDC.GetValue(k) - TDCOffsets[mTDC.GetChannel(k)]);
            si->SetSiliconType("X1");

            si->SetEnergyFront(energyi);
            si->SetEnergyBack(-1);
            si->SetTimeFront(mTDC.GetValue(k) - TDCOffsets[mTDC.GetChannel(k)]);
            si->SetTimeBack(-1);

            si->SetDetectorHit(X1DetHitNumber(i));
            si->SetStripFront(X1StripFront(i));
            si->SetStripBack(-1);

            si->SetADCChannelFront(i);
            si->SetADCChannelBack(-1);
            si->SetADCValueFront(ADC_import[i]);
            si->SetADCValueBack(-1);

            si->SetTDCChannelFront(mTDC.GetChannel(k));
            si->SetTDCChannelBack(-1);
            si->SetTDCValueFront(mTDC.GetValue(k));
            si->SetTDCValueBack(-1);

            si->SetMult(mTDC.GetMult(k));
          }
        }
      }
    }
  }

  si->SetHits(si->SizeOfEvent());
  if(!si->TestEvent())si->ClearEvent();
}

double X1EnergyCalc(int Channel, double ADCValue)
{
  double result = ADCOffsets[Channel] + ADCGains[Channel] * ADCValue;
  return result;
}

int X1DetHitNumber(int FrontChannel)
{
  int result = 0;
  for (int i = 0; i < NumberOfX1; i++)
  {
    if (FrontChannel >= X1ADCChannelLimits[i][0] && FrontChannel <= X1ADCChannelLimits[i][1])
    {
      result = i + 1;
    }
  }
  return result;
}

// The strip number runs 1->16 on each detector. i.e.
// There will be 4 strip 1s if there are 4 detectors.
// Use the detector hit quantity from above if you need to localise to one detector
int X1StripFront(int FrontChannel)
{
  // This one is just reversed.
  int result = 0;
  // Gives the number relative to the start of each detector. 0,16,32... return 0, 1, 17, 33 return 1 etc.
  result = FrontChannel % 16;
  // Want to start the channel numbering at 1 for consistency with the Micron documentation
  result = 16 - result;
  return result;
}

bool X1ADCTDCChannelTestPSide(int ADCChannel, int TDCChannel)
{
  bool result = false;
  for (int i = 0; i < NumberOfX1; i++)
  {
    // Check to see if the ADC/TDC events are in the same detector
    if (ADCChannel >= X1ADCChannelLimits[i][0] && ADCChannel <= X1ADCChannelLimits[i][1]
        && TDCChannel >= X1TDCChannelLimits[i][0] && TDCChannel <= X1TDCChannelLimits[i][1])
    {
      if (ADCChannel % 16 == TDCChannel % 16)
      {
        result = true;
      }
    }
  }

  return result;
}

bool X1TDCFrontTest(int TDCChannel)
{
  bool result = false;
  for (int i = 0; i < NumberOfX1; i++)
  {
    if (TDCChannel >= X1TDCChannelLimits[i][0] && TDCChannel <= X1TDCChannelLimits[i][1])
    {
      result = true;
    }
  }
  return result;
}

int X1TDCIdentifyDetector(int TDCFrontChannel)
{
  int result = -1;
  for (int i = 0; i < NumberOfX1; i++)
  {
    if (TDCFrontChannel >= X1TDCChannelLimits[i][0] && TDCFrontChannel <= X1TDCChannelLimits[i][1])
    {
      result = i;
    }
  }
  return result;
}

