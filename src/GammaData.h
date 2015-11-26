#ifndef __GammaData__
#define __GammaData__ 1

/*
 * 
 * This is a header file defining the data structure for ganna events for K600 analysis.
 * It should be used as a basis for another analysis file, an example of which should be found in the same directory.
 * 
 * Author: Philip Adsley, June 2014
 * E-mail: padsley@gmail.com
 * 
 * Please contact me if you find errors or have useful suggestions
 * 
 */

#include <vector>
#include <stdio.h>
#include <TCutG.h>
#include <string>

class GammaData
{
 public :
  GammaData();
  virtual ~GammaData();

 private : 
  int GammaHits;

  std::vector<double> GammaEnergy;
  std::vector<double> GammaTheta;
  std::vector<double> GammaPhi;
  std::vector<double> GammaTime;
  std::vector<std::string> GammaDetectorType;

  //There should follow some diagnostic values which will be added for The Greater Good (The Greater Good).
  std::vector<int> GammaADCChannel;
  std::vector<int> GammaADCValue;
  std::vector<int> GammaTDCChannel;
  std::vector<int> GammaTDCValue;

public:
  void SetHits(int hits){GammaHits = hits;}

  void SetEnergy(double ener){GammaEnergy.push_back(ener);}
  void SetTheta(double thet){GammaTheta.push_back(thet);}
  void SetPhi(double ph){GammaPhi.push_back(ph);}
  void SetTime(double tim){GammaTime.push_back(tim);}
  void SetDetectorType(std::string type){GammaDetectorType.push_back(type);}

  void SetADCChannel(int channel) { GammaADCChannel.push_back(channel); }
  void SetADCValue(int value) { GammaADCValue.push_back(value); }
  void SetTDCChannel(int channel) { GammaTDCChannel.push_back(channel); }
  void SetTDCValue(int value) { GammaTDCValue.push_back(value); }

  double GetEnergy(int i){return GammaEnergy.at(i);}
  double GetTheta(int i){return GammaTheta.at(i);}
  double GetPhi(int i){return GammaPhi.at(i);}
  double GetTime(int i){return GammaTime.at(i);}
  std::string GetType(int i){return GammaDetectorType.at(i);}

  void PrintEvent();
  bool TestEvent();
  void ClearEvent();

  unsigned int SizeOfEvent();
};

#endif
