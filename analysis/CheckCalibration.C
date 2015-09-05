#define CheckCalibration_cxx
// The class definition in CheckCalibration.h has been generated automatically
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
// Root > T->Process("CheckCalibration.C")
// Root > T->Process("CheckCalibration.C","some options")
// Root > T->Process("CheckCalibration.C+")
//

#include "CheckCalibration.h"
#include <TCanvas.h>
#include <TH2.h>
#include <TStyle.h>
static const int channels = 224;
static const int threshold = 500;

TH2F* uncalibrated;
#include "../analysis/SiliconParameters.c"
TH2F* calibrated;

void CheckCalibration::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
   uncalibrated = new TH2F("uncalibrated", "",
         4096, 0, 4096, channels, 0, channels);
   calibrated = new TH2F("calibrated", "",
         1000, 4000, 9000, channels, 0, channels);
}

void CheckCalibration::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

Bool_t CheckCalibration::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either CheckCalibration::GetEntry() or TBranch::GetEntry()
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
   b_RawInfo_ADCValues->GetEntry(entry);
   for (size_t i = 0; i < ADCValues.size(); ++i)
   {
      int val = ADCValues[i];
      if (val > threshold)
      {
         uncalibrated->Fill(val, i);
         calibrated->Fill(SiliconGain[i] * val + SiliconOffset[i], i);
      }
   }


   return kTRUE;
}

void CheckCalibration::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void CheckCalibration::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.
   gStyle->SetOptStat(0);
   TCanvas* c1 = new TCanvas("c1");
   c1->Divide(2, 1);
   c1->cd(1);
   uncalibrated->SetTitle("Uncalibrated ADC Values");
   uncalibrated->GetXaxis()->SetTitle("ADC Value");
   uncalibrated->GetYaxis()->SetTitleOffset(1.4);
   uncalibrated->GetYaxis()->SetTitle("ADC Channel");
   uncalibrated->Draw("col");
   c1->cd(2);
   calibrated->SetTitle("Calibrated ADC Values");
   calibrated->GetXaxis()->SetTitle("Silicon Energy/keV");
   calibrated->GetYaxis()->SetTitleOffset(1.4);
   calibrated->GetYaxis()->SetTitle("ADC Channel");
   calibrated->Draw("col");
   c1->Update();
}
