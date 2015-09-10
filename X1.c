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

extern Int_t t_tof;

static const double sigma = 30.;

void X1SiliconSort(SiliconData* si, float *ADC_import, int ntdc,
    int *TDC_channel_import, float *TDC_value_import)
{
  multiTDC mTDC;
  mTDC.multiTDCSort(ntdc, TDC_channel_import, TDC_value_import);

  //int tdcSize = mTDC.GetSize();

  //printf("mTDC.GetSize(): %d\n",mTDC.GetSize());

  for (int k = 0; k < mTDC.GetSize(); k++)
  {
    int tdcFront = mTDC.GetChannel(k);
    if (X1TDCFrontTest(tdcFront))
    {
      //	  if(W1TDCChannelLimits[int tdcBackSize =
      //for (int l = 0; l < mTDC.GetSize(); l++)
      {
	//int tdcBack = mTDC.GetChannel(l);
	//if (W1TDCBackTest(-1) && W1TDCFrontBackTest(tdcFront, -1))
	{
	  int DetNum = X1TDCIdentifyDetector(tdcFront);
	  if (DetNum > 0 && DetNum != 3)
	  {
	    for (int i = X1ADCChannelLimits[DetNum][0]; i <= X1ADCChannelLimits[DetNum][1]; i++)
	    {
	      if (X1ADCTDCChannelTestPSide(i, tdcFront) && ADC_import[i] > ADCPedestals[i])
	      {
		double energyi = X1EnergyCalc(i,ADC_import[i]);
		if (energyi < 200 || energyi > 7500)
		  continue;
		//Test whether the hits are in the front and back of the same detector and whether the energies are good
		//				      if(W1FrontBackTest(i,j,energyi,energyj,si) && W1ADCTDCChannelTestNSide(j,tdcBack))
		//printf("through front-back test\n");

		si->SetEnergy(energyi);          //Just use front energy because the back energy resolution is bloody terrible
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
    }
  }

  si->SetHits(si->SizeOfEvent());
  if(!si->TestEvent())si->ClearEvent();
  //si->PrintEvent();
}

void X1Init()//Initialise function which gets the information on the DAQ channels->Physical channels
{

}

bool X1SuppressChannel(int Channel)//If the ADC channel is one which we wish to suppress, we do that here. Use if(Channel = 12)return true to suppress channel 12. Load of else ifs for the other suppressed channels. Then else return false.
{
  if(Channel>128)
    return true;
  else
    return false;
}

double X1EnergyCalc(int Channel, double ADCValue)
{
  //printf("Channel: %i \t ADCValue: %f\n",Channel,ADCValue);
  //printf("Offset: %f \t Gain: %f\n",ADCOffsets[Channel],ADCGains[Channel]);
  double result = ADCOffsets[Channel] + ADCGains[Channel]*ADCValue;
  return result;
}

int X1DetHitNumber(int FrontChannel)
{
  int result = 0;
  for(int i=0;i<NumberOfX1;i++)
  {
    if(FrontChannel>=X1ADCChannelLimits[i][0] && FrontChannel<=X1ADCChannelLimits[i][1])//If the two hits are part of the front and back of the same detector, then consider that the event might be good - make sure that hit 'i' in the main loop in the sort is always the front. Then we don't get any double-counting
    {
      result = i+1;
    }
  }
  return result;
}

int X1StripFront(int FrontChannel)//The strip number runs 1->16 on each detector. i.e. there will be 4 strip 1s if there are 4 detectors. Use the detector hit quantity from above if you need to localise to one detector
{
  //This one is just reversed.
  int result = 0;
  result = FrontChannel%16;//Gives the number relative to the start of each detector. 0,16,32... return 0, 1, 17, 33 return 1 etc.
  result = 16-result;//Want to start the channel numbering at 1 for consistency with the Micron documentation
  return result;
}

bool X1ADCTDCChannelTestPSide(int ADCChannel, int TDCChannel)
{
  bool result = false;
  for(int i=0;i<NumberOfX1;i++)
  {
    if(ADCChannel>=X1ADCChannelLimits[i][0] && ADCChannel<=X1ADCChannelLimits[i][1] && TDCChannel>=X1TDCChannelLimits[i][0] && TDCChannel<=X1TDCChannelLimits[i][1])//Check to see if the ADC/TDC events are in the same detector
    {
      if(ADCChannel%16==TDCChannel%16)
      {
	result = true;
      }
      //if(TDCChannel<832)printf("Pass for invalid TDC value! ADCChannel: %d \t TDC Channel: %d\n",ADCChannel, TDCChannel);
    }
    if(X1ADCChannelLimits[i][0]==-1)result = true; //No information for some of the W1s -> Suppress this test (i.e. if there is no ADC channel set)
    if(X1ADCChannelLimits[i][1]==-1)result = true;
    if(X1TDCChannelLimits[i][0]==-1)result = true;
    if(X1TDCChannelLimits[i][1]==-1)result = true;
  }

  //printf("W1ADCTDCChannelTest stop: %d %d",ADCChannel,TDCChannel);
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
    if (X1TDCChannelLimits[i][0] == -1) result = true;
    if (X1TDCChannelLimits[i][1] == -1) result = true;
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

