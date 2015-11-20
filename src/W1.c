/*
 * Sort code for the W1s in conjunction with the K600 spectrometer
 * This code 'plugs in' to f-plane.c and is called from there. It populates the SiliconData class in that code and chucks the data out into the ROOT tree 'DATA'.
 *
 * Date: August 2014
 *
 */

#include "SiliconData.h"
#include "W1.h"

extern int ADCModules;
extern int ADCsize;
extern int NumberOfW1;

extern int **W1ADCChannelLimits;
extern int **W1TDCChannelLimits;

extern double *ADCOffsets;
extern double *ADCGains;
extern int *ADCPedestals;
extern double *TDCOffsets;

SiliconData *W1SiliconSort(float *ADC_import, int ntdc,
    int *TDC_channel_import, float *TDC_value_import)
{
  SiliconData *si = new SiliconData();

  multiTDC mTDC;
  mTDC.multiTDCSort(ntdc, TDC_channel_import, TDC_value_import);

  for (int k = 0; k < mTDC.GetSize(); k++)
  {
    int tdcFront = mTDC.GetChannel(k);
    if (W1TDCFrontTest(tdcFront))
    {
      int DetNum = W1TDCIdentifyDetector(tdcFront, -1);
      if (DetNum > 0)
      {
        double value = mTDC.GetValue(k);
        value -= TDCOffsets[mTDC.GetChannel(k)];
        for (int i = W1ADCChannelLimits[DetNum][0]; i <= W1ADCChannelLimits[DetNum][1]; i++)
        {
          if (W1ADCTDCChannelTestPSide(i, tdcFront) && ADC_import[i] > ADCPedestals[i])
          {
            for (int j = W1ADCChannelLimits[DetNum][2]; j <= W1ADCChannelLimits[DetNum][3]; j++)
            {
              if (ADC_import[j] > ADCPedestals[j])
              {
                double energyi = W1EnergyCalc(i,ADC_import[i]);
                double energyj = W1EnergyCalc(j,ADC_import[j]);
                //Test whether the hits are in the front and back of the same detector and whether the energies are good
                if (W1FrontBackTest(i, j, energyi, energyj, si))
                {
                  //Just use front energy because the back energy resolution is bloody terrible
                  si->SetEnergy(energyi);
                  si->SetTheta(W1ThetaCalc(i, j));
                  si->SetPhi(W1PhiCalc(i, j));
                  si->SetTime(mTDC.GetValue(k) - TDCOffsets[mTDC.GetChannel(k)]);
                  si->SetSiliconType("W1");

                  si->SetEnergyFront(energyi);
                  si->SetEnergyBack(energyj);
                  si->SetTimeFront(mTDC.GetValue(k) - TDCOffsets[mTDC.GetChannel(k)]);
                  si->SetTimeBack(mTDC.GetValue(k));

                  si->SetDetectorHit(W1DetHitNumber(i, j));
                  si->SetStripFront(W1StripFront(i));
                  si->SetStripBack(W1StripBack(j));

                  si->SetADCChannelFront(i);
                  si->SetADCChannelBack(j);
                  si->SetADCValueFront(ADC_import[i]);
                  si->SetADCValueBack(ADC_import[j]);

                  si->SetTDCChannelFront(mTDC.GetChannel(k));
                  si->SetTDCChannelBack(-1);
                  si->SetTDCValueFront(mTDC.GetValue(k));
                  si->SetTDCValueBack(mTDC.GetValue(k));

                  si->SetMult(mTDC.GetMult(k));
                }
              }
            }
          }
        }
      }
    }
  }

  si->SetHits(si->SizeOfEvent());
  if(!si->TestEvent())si->ClearEvent();
  return si;
}

double W1EnergyCalc(int Channel, double ADCValue)
{
  double result = ADCOffsets[Channel] + ADCGains[Channel] * ADCValue;
  return result;
}

double W1ThetaCalc(int FrontChannel, int BackChannel)
{
  double result = 180;
  return result;
}

double W1PhiCalc(int FrontChannel, int BackChannel)
{
  double result = 0;
  return result;
}

static const double sigma = 200.;

bool W1FrontBackTest(int FrontChannel, int BackChannel, double FrontEnergy, double BackEnergy, SiliconData *si)
{
  bool result = false;

  for (int i = 0; i < NumberOfW1; i++)
  {
    //If the two hits are part of the front and back of the same detector,
    // then consider that the event might be good - make sure that hit 'i'
    // in the main loop in the sort is always the front.
    // Then we don't get any double-counting
    if (FrontChannel >= W1ADCChannelLimits[i][0] && FrontChannel <= W1ADCChannelLimits[i][1]
        && BackChannel >= W1ADCChannelLimits[i][2] && BackChannel <= W1ADCChannelLimits[i][3])
    {
      double diff = FrontEnergy - BackEnergy;
      // Test if front and back energies are approximately equal
      if (true || std::abs(diff) < 3*sigma)
      {
        //They are -> it's a good event
        result = true;
      }
    }
  }

  return result;
}


int W1DetHitNumber(int FrontChannel, int BackChannel)
{
  int result = 0;
  for (int i=0; i < NumberOfW1; i++)
  {
    if (FrontChannel >= W1ADCChannelLimits[i][0] && FrontChannel <= W1ADCChannelLimits[i][1]
        && BackChannel >= W1ADCChannelLimits[i][2] && BackChannel <= W1ADCChannelLimits[i][3])
    {
      result = i+1;
    }
  }
  return result;
}

// The strip number runs 1->16 on each detector. i.e. there will be 4 strip 1s
// if there are 4 detectors. Use the detector hit quantity from above if you need to localise to one detector
int W1StripFront(int FrontChannel)
{
  //This one is just reversed.
  int result = 0;
  //Gives the number relative to the start of each detector. 0,16,32... return 0, 1, 17, 33 return 1 etc.
  result = FrontChannel % 16;
  //Want to start the channel numbering at 1 for consistency with the Micron documentation
  result = 16 - result;
  return result;
}

int W1StripBack(int BackChannel)//Again, for the W1, this runs from 1->16.
{
  int result = 0;
  int chan = BackChannel % 16;
  if (chan >= 8) result = chan - 7; //(actually -8+1 because we want to shift 8->15 to 0->7 and then 1->8)
  else if (chan <= 7) result = chan + 8 + 1;
  return result;
}

bool W1ADCTDCChannelTestPSide(int ADCChannel, int TDCChannel)
{
  bool result = false;
  for (int i = 0; i < NumberOfW1; i++)
  {
    //Check to see if the ADC/TDC events are in the same detector
    if (ADCChannel >= W1ADCChannelLimits[i][0] && ADCChannel <= W1ADCChannelLimits[i][1]
        && TDCChannel >= W1TDCChannelLimits[i][0] && TDCChannel <= W1TDCChannelLimits[i][1])
    {
      if(ADCChannel % 16 == TDCChannel % 16)
      {
        result = true;
      }
    }
    //No information for some of the W1s -> Suppress this test (i.e. if there is no ADC channel set)
    if (W1ADCChannelLimits[i][2] == -1) result = true;
    if (W1ADCChannelLimits[i][3] == -1) result = true;
  }
  return result;
}

bool W1ADCTDCChannelTestNSide(int ADCChannel, int TDCChannel)
{
  bool result = false;
  for (int i = 0; i < NumberOfW1; i++)
  {
    //Check to see if the ADC/TDC events are in the same detector
    if (ADCChannel >= W1ADCChannelLimits[i][2] && ADCChannel <= W1ADCChannelLimits[i][3]
        && TDCChannel >= W1TDCChannelLimits[i][2] && TDCChannel <= W1TDCChannelLimits[i][3])
    {
      if (ADCChannel % 16 == TDCChannel % 16)
      {
        result = true;
      }
    }
    //No information for some of the W1s -> Suppress this test (i.e. if there is no ADC channel set)
    if (W1TDCChannelLimits[i][2] == -1) result = true;
    if (W1TDCChannelLimits[i][3] == -1) result = true;
  }

  return result;
}

bool W1TDCFrontTest(int TDCChannel)
{
  bool result = false;
  for (int i = 0; i < NumberOfW1; i++)
  {
    if (TDCChannel >= W1TDCChannelLimits[i][0] && TDCChannel <= W1TDCChannelLimits[i][1])
    {
      result = true;
    }
  }
  return result;
}

bool W1TDCBackTest(int TDCChannel)
{
  bool result = false;
  for (int i = 0; i < NumberOfW1; i++)
  {
    if (TDCChannel >= W1TDCChannelLimits[i][2] && TDCChannel <= W1TDCChannelLimits[i][3])
    {
      result = true;
    }
    if (W1TDCChannelLimits[i][2] == -1) result = true;
    if (W1TDCChannelLimits[i][3] == -1) result = true;
  }
  return result;
}

bool W1TDCFrontBackTest(int TDCFrontChannel, int TDCBackChannel)
{
  bool result = false;
  for (int i = 0; i < NumberOfW1; i++)
  {
    if (TDCFrontChannel >= W1TDCChannelLimits[i][0] && TDCFrontChannel <= W1TDCChannelLimits[i][1]
        && TDCBackChannel >= W1TDCChannelLimits[i][2] && TDCBackChannel <= W1TDCChannelLimits[i][3])
    {
      result = true;
    }
    if (W1TDCChannelLimits[i][2] == -1) result = true;
    if (W1TDCChannelLimits[i][3] == -1) result = true;
  }
  return result;
}

int W1TDCIdentifyDetector(int TDCFrontChannel, int TDCBackChannel)
{
  int result = -1;
  for (int i = 0; i < NumberOfW1; i++)
  {
    if (TDCFrontChannel >= W1TDCChannelLimits[i][0] && TDCFrontChannel <= W1TDCChannelLimits[i][1])
    {
      if (W1TDCChannelLimits[i][2] == -1 || (TDCBackChannel >= W1TDCChannelLimits[i][2]
            && TDCFrontChannel <= W1TDCChannelLimits[i][3]))
      {
        result = i;
      }
    }
  }
  return result;
}

