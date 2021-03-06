#define TDCAlign_cxx
// The class definition in analysis/TDCAlign.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// Root > T->Process("analysis/TDCAlign.C")
// Root > T->Process("analysis/TDCAlign.C","some options")
// Root > T->Process("analysis/TDCAlign.C+")
//

#include "TDCAlign.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <TCanvas.h>
#include <TCutG.h>
#include <TH1.h>
#include <TH2.h>
#include <TParameter.h>
#include <TStyle.h>

#include "config.h"

static const int h_bins = 1000;
static const int h_start = -10000;
static const int h_end = 10000;

void TDCAlign::Begin(TTree * /*tree*/)
{
	// The Begin() function is called at the start of the query.
	// When running with PROOF Begin() is only called on the client.
	// The tree argument is deprecated (on PROOF 0 is passed).
	AnalysisConfig& config = AnalysisConfig::Instance();

    energy_gate = config.UseEnergyGate();
    energy_min = config.EnergyMin();
	embedPID = false;
	CUTpid = NULL;

    if (config.UsePID())
    {
        if (config.PIDfile() == "TREE")
        {
            embedPID = true;
        }
        else
        {
            TFile* pidfile = new TFile(config.PIDfile() + TString(".root"), "OLD");
            CUTpid = (TCutG*)pidfile->Get("CUTpid");
            delete pidfile;
        }
    }

	if (fInput)
	{
        fInput->Add(
                new TParameter<bool>("energy_gate", energy_gate)
                );
        fInput->Add(
                new TParameter<double>("energy_min", energy_min)
                );
		fInput->Add(
				new TParameter<bool>("embedPID", embedPID)
				);
		if (CUTpid)
		{
			fInput->Add(CUTpid);
		}
	}
}

void TDCAlign::SlaveBegin(TTree * /*tree*/)
{
	// The SlaveBegin() function is called after the Begin() function.
	// When running with PROOF SlaveBegin() is called on each slave server.
	// The tree argument is deprecated (on PROOF 0 is passed).

	TString option = GetOption();

	if (fInput)
	{
        energy_gate = ((TParameter<bool>*)fInput->FindObject("energy_gate"))->GetVal();
        energy_min = ((TParameter<double>*)fInput->FindObject("energy_min"))->GetVal();
		embedPID = ((TParameter<bool>*)fInput->FindObject("embedPID"))->GetVal();
		CUTpid = (TCutG*)fInput->FindObject("CUTpid");
	}

	for (size_t i = 0; i < channels; ++i)
	{
		std::stringstream buf;
		buf << "tdc" << i;
		tdc[i] = new TH1F(buf.str().c_str(), "", h_bins, h_start, h_end);
		fOutput->Add(tdc[i]);
	}
}

Bool_t TDCAlign::Process(Long64_t entry)
{
	// The Process() function is called for each entry in the tree (or possibly
	// keyed object in the case of PROOF) to be processed. The entry argument
	// specifies which entry in the currently loaded tree is to be processed.
	// It can be passed to either analysis/TDCAlign::GetEntry() or TBranch::GetEntry()
	// to read either all or the required parts of the data. When processing
	// keyed objects with PROOF, the object is already loaded and is available
	// via the fObject pointer.
	//
	// This function should contain the "body" of the analysis. It can contain
	// simple or elaborate selection criteria, run algorithms on the data
	// of the event and typically fill histograms.
	//
	// The processing can be stopped by calling Abort().
	//
	// Use fStatus to set the return value of TTree::Process().
	//
	// The return value is currently not used.
	b_t_tof->GetEntry(entry);
	b_SiliconInfo_TDCChannelFront->GetEntry(entry);
	b_SiliconInfo_TDCValueFront->GetEntry(entry);
	b_SiliconInfo_SiliconEnergy->GetEntry(entry);
	b_t_pad1->GetEntry(entry);
	b_t_PIDgood->GetEntry(entry);

	if (CUTpid && !CUTpid->IsInside(tof, pad1))
		return kTRUE;
	if (embedPID && !PIDgood)
		return kTRUE;

	for (size_t i = 0; i < TDCChannelFront.size(); ++i)
	{
		if (energy_gate && SiliconEnergy[i] < energy_min)
			continue;
		if (TDCChannelFront[i] >= channel_start && TDCChannelFront[i] < channel_end)
		{
			tdc[TDCChannelFront[i] - channel_start]->Fill(
					TDCValueFront[i] - tof
			);
		}
	}

	return kTRUE;
}

void TDCAlign::SlaveTerminate()
{
	// The SlaveTerminate() function is called after all entries or objects
	// have been processed. When running with PROOF SlaveTerminate() is called
	// on each slave server.

}

void TDCAlign::Terminate()
{
	// The Terminate() function is the last function to be called during
	// a query. It always runs on the client, it can be used to present
	// the results graphically or save the results to file.
	int bin0 = 0;
	std::cout << "Alignment bin is: " << bin0 << std::endl;
	std::ofstream output;
	output.open("../../output/TDCAlignPR228B.dat");
	using std::endl;
	TCanvas* c1 = new TCanvas("silicon times");
	for (size_t i = 0; i < channels; ++i)
	{
		std::stringstream buf;
		buf << "tdc" << i;
		tdc[i] = (TH1F*)fOutput->FindObject(buf.str().c_str());
		tdc[i]->Draw();
		c1->SaveAs("../../output/align/TDCChannel" + TString::LLtoa(i, 10) + ".png");
		int bin = tdc[i]->GetMaximumBin();
		bin = tdc[i]->GetXaxis()->GetBinCenter(bin);
		int offset = bin - bin0;
		output << i + channel_start << " " << offset << endl;
	}
	output << "eof";
	output.close();
}
