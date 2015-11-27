#include "RawData.h"

extern int ADCsize;
extern int TDCsize;
extern int QDCsize;

extern double *ADCOffsets;
extern double *ADCGains;
extern double *TDCOffsets;

RawData::RawData()
{

}

RawData::~RawData()
{

}

void RawData::Init(RawData *raw)
{
  raw->SetADCSize(raw, ADCsize);
  raw->SetTDCSize(raw, TDCsize);
  raw->SetQDCSize(raw, QDCsize);
}

void RawData::SetADCSize(RawData *raw, int asize)
{
  raw->ADCChannels.resize(asize);
  raw->ADCValues.resize(asize);
  raw->ADCCalibratedValues.resize(asize);
}

void RawData::SetTDCSize(RawData *raw, int tsize)
{
  raw->TDCValues.resize(tsize);
  raw->TDCChannels.resize(tsize);
  raw->TDCCalibratedValues.resize(tsize);
}

void RawData::SetQDCSize(RawData *raw, int qsize)
{
  raw->QDCValues.resize(qsize);
}

RawData *RawDataDump(float *ADC_import, int ntdc, int *TDC_channel_import, float *TDC_value_import, float *QDC_import)
{
  RawData *raw = new RawData();
  raw->Init(raw);
  for (int i = 0; i < ADCsize; i++)
    raw->SetADCValue(i, ADC_import[i]);
  for (int i = 0; i < ADCsize; i++)
    raw->SetADCCalibratedValue(i, ADCOffsets[i] + ADCGains[i] * ADC_import[i]);

  for (int n = 0; n < ntdc; n++)
  {
    raw->SetTDCChannel(n, TDC_channel_import[n]);
    raw->SetTDCValue(n, TDC_value_import[n]);
    raw->SetTDCCalibratedValue(n, TDC_value_import[n] - TDCOffsets[TDC_channel_import[n]]);
  }

  for (int i = 0; i < QDCsize; i++)
    raw->SetQDCValue(i, QDC_import[i]);

  return raw;
}
