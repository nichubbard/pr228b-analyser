#include "LEPS.h"

extern int ADCsize;

extern int ADCModules;
extern int ADCsize;
extern int NumberOfLEPS;

extern int *LEPSADCChannelLimits;
extern int *LEPSTDCChannelLimits;

extern double *ADCOffsets;
extern double *ADCGains;
extern int *ADCPedestals;
extern double *TDCOffsets;

void LEPSSort(GammaData* gd, float *ADC_import, int ntdc, int *TDC_channel_import, float *TDC_value_import)
{
	multiTDC mTDC;
	mTDC.multiTDCSort(ntdc, TDC_channel_import, TDC_value_import);

	for (int k = 0; k < mTDC.GetSize(); k++)
	{
		int tdcFront = mTDC.GetChannel(k);
		int i = LEPSADCChannelFind(tdcFront);
		if (i != -1)
		{
			if (ADC_import[i] > ADCPedestals[i])
			{
				double energyi = LEPSEnergyCalc(i,ADC_import[i]);
				gd->SetEnergy(energyi);
				gd->SetTheta(-1);
				gd->SetPhi(-1);
				gd->SetTime(mTDC.GetValue(k) - TDCOffsets[mTDC.GetChannel(k)]);
				gd->SetDetectorType("LEPS");

				gd->SetADCChannel(i);
				gd->SetADCValue(ADC_import[i]);
				gd->SetTDCChannel(mTDC.GetChannel(k));
				gd->SetTDCValue(mTDC.GetValue(k));
			}
		}
	}
	gd->SetHits(gd->SizeOfEvent());
	mTDC.ClearEvent();
}

double LEPSEnergyCalc(int Channel, double ADCValue)
{
	double result = ADCOffsets[Channel] + ADCGains[Channel]*ADCValue;
	return result;
}

int LEPSADCChannelFind(int TDCChannel)
{
	for (int i = 0; i < NumberOfLEPS; ++i)
	{
		if (TDCChannel == LEPSTDCChannelLimits[i])
			return LEPSADCChannelLimits[i];
	}
	return -1;
}
