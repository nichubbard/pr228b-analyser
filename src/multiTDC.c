#include "multiTDC.h"

extern int TDCModules;
extern float *ADC;
extern int *ChannelCounter;
extern int *GoodChannelCounter;

#include <cstring>

//This code is mainly dealing with TDC data in ancillary detector channels, not with the main focal plane TDC stuff which is dealt with in f-plane.c and was written by Retief. I (Phil) know very little about how that bit works and so will try to avoid doing anything here to break that.

void multiTDC::multiTDCSort(int ntdc, int *TDC_channel_import, float *TDC_value_import)
{
	TDChits = 0;
	int TDCsize = 128 * TDCModules;
	std::memset(ChannelCounter, 0, TDCsize * sizeof(int));
	std::memset(GoodChannelCounter, 0, TDCsize * sizeof(int));

	for(int n=0;n<ntdc;n++)
	{
		int channel = TDC_channel_import[n];
		if (!(channel > 0 && channel < TDCsize))
			continue;
		ChannelCounter[channel] += 1;

		if (TDC_value_import[n] > PulseLimits[0] &&
				TDC_value_import[n] < PulseLimits[1])
		{
			GoodChannelCounter[channel] += 1;
		}
	}

	for(int n=0;n<ntdc;n++)//Loop over and dispose of the simple events (the single hit events)
	{
		int channel = TDC_channel_import[n];
		float value = TDC_value_import[n];
		if (channel > 943 && channel < TDCsize)
		{
			if (ChannelCounter[channel] == 0)
			{
				printf("Something has gone wrong - the number of counts for channel %d is expected to be zero but something (%d) is seen.\n",TDC_channel_import[n],ChannelCounter[TDC_channel_import[n]]);
				for(int nn=0;nn<ntdc;nn++){printf("Dump: \t ntdc: %d \t n: %d \t TDC_channel_import[n]: %d \t ChannelCounter[TDC_channel_import[n]]: %d \t TDC_value_import[n]: %f \n",
						ntdc,nn,TDC_channel_import[nn],ChannelCounter[TDC_channel_import[nn]],TDC_value_import[nn]);}	
			}
			else if (ChannelCounter[channel] == 1)
			{
				//The reason that we do this this way is to look at how many events fall outside the good beampulse - only when we have multiple hits do we need to worry about the multiple hits and this should be quicker 
				SetChannel(channel);
				SetValue(value);
				SetMult(ChannelCounter[channel]);
				TDChits++;
				//printf("\n ChannelCounter[%d]==1 \n",TDC_channel_import[n]);
			}
			else if (ChannelCounter[channel] > 1 && GoodChannelCounter[channel] == 1)
			{
				if (value > PulseLimits[0] && value < PulseLimits[1])
				{
					SetChannel(channel);
					SetValue(value);
					SetMult(ChannelCounter[channel]);
					TDChits++;
					ChannelCounter[channel] *= -1;
				}
			}
			//       else if(ChannelCounter[TDC_channel_import[n]]>1 && GoodChannelCounter[TDC_channel_import[n]]==2
			else if (ChannelCounter[channel] > 1 && GoodChannelCounter[channel] > 1)
			{
				printf("The number of TDC hits within the user-defined 'good pulse' is greater than 1; the number of hits is %d. The code currently doesn't deal with this.\n",GoodChannelCounter[TDC_channel_import[n]]);
				printf(" Pulse channel: %d, value: %f\n", channel, value);
			}
			else if (ChannelCounter[channel] > 1 && GoodChannelCounter[channel] == 0)
			{
				SetChannel(channel);
				SetValue(value);
				SetMult(ChannelCounter[channel]);
				TDChits++;
				//Take the first TDC event f
				//Now set the ChannelCounter for the channel to zero as we don't want to take any more of these events
				//this forces the change above in the first part of the if chain.
				//Need to fix at some point.or a channel where nothing falls within the good beampulse
				ChannelCounter[channel]*=-1;
			}
		}
	}

	SetHits(TDChits);
	//delete [] ChannelCounter;
	//delete [] GoodChannelCounter;
	//printf("mTDC: L75\n");
}

multiTDC::multiTDC()
{
	//   printf("multiTDC\n");
	//   printf("\n multiTDC.c:L74 \n");
	//SetHits(TDChits);//printf("\n multiTDC.c:L75 \n");
	//delete ChannelCounter;//printf("\n multiTDC.c:L76 \n");
	//delete GoodChannelCounter;printf("\n multiTDC.c:L77 \n");
}

multiTDC::~multiTDC()
{
	//printf("~multiTDC()\n");
}

void multiTDC::PrintEvent()
{
	printf("Size of event: %u\n", SizeOfEvent());
	TDCchannels.clear();
	TDCvalues.clear();
}

bool multiTDC::TestEvent()
{
	bool result = true;
	if(TDChits != TDCchannels.size()){printf("TDChits != TDCchannels.size()\n");result = false;}
	if(TDChits != TDCvalues.size()){printf("TDChits != TDCvalues.size()\n");result = false;}
	if(TDCchannels.size() != TDCvalues.size()){printf("TDCchannels.size() != TDCvalues.size()\n");result = false;}
	return result;
}

void multiTDC::ClearEvent()
{
	TDChits=0;
	TDCchannels.clear();
	TDCvalues.clear();
}

unsigned int multiTDC::SizeOfEvent()
{
	return TDChits;
}
