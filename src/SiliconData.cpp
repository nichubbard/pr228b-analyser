#include "SiliconData.h"

SiliconData::SiliconData()
{
}

SiliconData::~SiliconData()
{
  ClearEvent();
}

void SiliconData::PrintEvent()
{
  printf("Silicon hits: %d\n", SizeOfEvent());

  for (unsigned int i = 0; i < SizeOfEvent(); i++)
  {
	printf("Hit number: %d\n", i);
	printf("Energy: %g\n", GetEnergy(i));
  }
}

// Test the output data: do we have the same number of hits in each thing (energy, time etc.)
// and are the events good and shizzle like that
bool SiliconData::TestEvent()
{
  bool result = true;
  unsigned int hits = SiliconEnergy.size();

  if(SiliconTheta.size() != hits){printf("Mismatched size: Theta\n"); result = false;}
  if(SiliconPhi.size() != hits){printf("Mismatched size: Phi\n"); result = false;}
  if(SiliconTime.size() != hits){printf("Mismatched size: Time\n"); result = false;}
  if(SiliconTimeFront.size() != hits){printf("Mismatched size: Front Time\n"); result = false;}
  if(SiliconTimeBack.size() != hits){printf("Mismatched size: Back Time\n"); result = false;}

  if(DetectorHit.size() != hits){printf("Mismatched size: DetectorHit"); result = false;}
  if(SiliconTime.size() != hits){printf("Mismatched size: SiliconTime"); result = false;}
  if(ADCChannelFront.size() != hits){printf("Mismatched size: ADCChannelFront\n"); result = false;}
  if(ADCChannelBack.size() != hits){printf("Mismatched size: ADCChannelBack\n"); result = false;}
  if(TDCChannelFront.size() != hits){printf("Mismatched size: TDCChannelFront\n"); result = false;}
  if(TDCChannelBack.size() != hits){printf("Mismatched size: TDCChannelBack\n"); result = false;}
  if(ADCValueFront.size() != hits){printf("Mismatched size: ADCValueFront\n"); result = false;}
  if(ADCValueBack.size() != hits){printf("Mismatched size: ADCValueBack\n"); result = false;}
  if(TDCValueFront.size() != hits){printf("Mismatched size: TDCValueFront\n"); result = false;}
  if(TDCValueBack.size() != hits){printf("Mismatched size: TDCValueBack\n"); result = false;}
  if(StripFront.size() != hits){printf("Mismatched size: StripFront\n"); result = false;}
  if(StripBack.size() != hits){printf("Mismatched size: StripBack\n"); result = false;}
  if(EnergyFront.size() != hits){printf("Mismatched size: EnergyFront\n"); result = false;}
  if(EnergyBack.size() != hits){printf("Mismatched size: EnergyBack\n"); result = false;}

  return result;
}

void SiliconData::ClearEvent()
{
  SiliconEnergy.clear();
  SiliconTheta.clear();
  SiliconPhi.clear();
  SiliconTime.clear();

  DetectorHit.clear();
  ADCChannelFront.clear();
  ADCChannelBack.clear();
  TDCChannelFront.clear();
  TDCChannelBack.clear();
  ADCValueFront.clear();
  ADCValueBack.clear();
  TDCValueFront.clear();
  TDCValueBack.clear();
  StripFront.clear();
  StripBack.clear();
  EnergyFront.clear();
  EnergyBack.clear();
}

unsigned int SiliconData::SizeOfEvent()
{
  unsigned int result = SiliconEnergy.size();
  return result;
}

