#define Li2CO3_cxx
// The class definition in Li2CO3.h has been generated automatically
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
// Root > T->Process("Li2CO3.C")
// Root > T->Process("Li2CO3.C","some options")
// Root > T->Process("Li2CO3.C+")
//

#include "Li2CO3.h"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <TCanvas.h>
#include <TF1.h>
#include <TLine.h>
#include <TList.h>
#include <TParameter.h>
#include <TTree.h>
#include <TStyle.h>
#include <vector>

using std::size_t;

/*
 *static const int tdc_gate_left = -1600;
 *static const int tdc_gate_right = -1200;
 */

static const int tdc_gate_left = 1200;
static const int tdc_gate_right = 2000;

static const double x1_oxygen_left = 6.0;
static const double x1_oxygen_right = 6.2;
static const double x1_oxygen_top = 1700;
static const double x1_oxygen_bottom = 0;
static const double x1_background_l = 5.8;
static const double x1_background_r = 6.4;

/*
 *static const double x1_oxygen_left = 11.95;
 *static const double x1_oxygen_right = 12.15;
 *static const double x1_oxygen_top = 3100;
 *static const double x1_oxygen_bottom = 2400;
 *static const double x1_background_l = 11.8;
 *static const double x1_background_r = 12.3;
 */

void Li2CO3::Begin(TTree * /*tree*/)
{
    // The Begin() function is called at the start of the query.
    // When running with PROOF Begin() is only called on the client.
    // The tree argument is deprecated (on PROOF 0 is passed).

    TString option = GetOption();
    TFile* cutfile = new TFile("CUTpid1098.root", "OLD");
    TFile* cutx1 = new TFile("CUTX1tof1098.root", "OLD");
    CUTpid = (TCutG*)cutfile->Get("CUTpid");
    CUTX1tof = 0;
    CUTX1tof = (TCutG*)cutx1->Get("CUTX1tof");

    if (fInput)
    {
        fInput->Add(CUTpid);
        fInput->Add(CUTX1tof);
    }
    delete cutfile;
    delete cutx1;
}

void Li2CO3::SlaveBegin(TTree * /*tree*/)
{
    // The SlaveBegin() function is called after the Begin() function.
    // When running with PROOF SlaveBegin() is called on each slave server.
    // The tree argument is deprecated (on PROOF 0 is passed).

    TString option = GetOption();

    spectrometer = new TH1F("spectrometer", "", 800, 4, 16);
    silicontime = new TH1F("silicontime", "", 1500, 0, 4500);
    raw = new TH2F("raw", "", 800, 4, 16, 1000, 0, 10000);
    gated = new TH2F("gated", "", 800, 4, 16, 1000, 0, 10000);
    gated_coinc = new TH2F("gated_coinc", "", 800, 4, 16, 1000, 0, 10000);
    mpcty = new TH2F("multiplicity", "", 800, 4, 16, 160, 0, 160);
    fOutput->Add(spectrometer);
    fOutput->Add(silicontime);
    fOutput->Add(raw);
    fOutput->Add(gated);
    fOutput->Add(gated_coinc);
    fOutput->Add(mpcty);

    if (fInput)
    {
        CUTpid = (TCutG*)fInput->FindObject("CUTpid");
        CUTX1tof = (TCutG*)fInput->FindObject("CUTX1tof");
    }

    stats_spec_total = 0;
    stats_spec_accepted = 0;
    stats_pid = 0;
    stats_pid2 = 0;
    stats_x1_chisq = 0;
    stats_x1_flag = 0;
    stats_x2_flag = 0;
    stats_u1_flag = 0;
    stats_u2_flag = 0;
    stats_y1 = 0;
    stats_y2 = 0;
    stats_thscat = 0;

    stats_si_total = 0;
    stats_si_accepted = 0;
    stats_si_tdc_rejected = 0;
    stats_si_energy_rejected =  0;
}

Bool_t Li2CO3::Process(Long64_t entry)
{
    // The Process() function is called for each entry in the tree (or possibly
    // keyed object in the case of PROOF) to be processed. The entry argument
    // specifies which entry in the currently loaded tree is to be processed.
    // It can be passed to either Li2CO3::GetEntry() or TBranch::GetEntry()
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


    b_SiliconInfo_SiliconEnergy->GetEntry(entry);
    b_SiliconInfo_DetectorHit->GetEntry(entry);
    b_t_Ex->GetEntry(entry);
    b_t_X1pos->GetEntry(entry);
    b_SiliconInfo_SiliconTime->GetEntry(entry);
    b_t_X1chisq->GetEntry(entry);
    b_t_X1flag->GetEntry(entry);
    b_t_X2flag->GetEntry(entry);
    b_t_U1flag->GetEntry(entry);
    b_t_U2flag->GetEntry(entry);
    b_t_tof->GetEntry(entry);
    b_t_pad1->GetEntry(entry);
    b_t_pad2->GetEntry(entry);
    b_t_Y1->GetEntry(entry);
    b_t_Y2->GetEntry(entry);
    b_t_ThSCAT->GetEntry(entry);

    std::vector<int> filteredDhit;
    std::vector<double> filteredE;

    stats_spec_total++;
    if (X1chisq < 0 || X1chisq >= 0.2) {
        stats_x1_chisq++;
        return kTRUE;
    }
    if (X1flag != 0) {
        stats_x1_flag++;
        return kTRUE;
    }
    if (X2flag != 0) {
        stats_x2_flag++;
        return kTRUE;
    }
    if (U1flag != 0) {
        stats_u1_flag++;
        return kTRUE;
    }
    if (U2flag != 0) {
        stats_u2_flag++;
        return kTRUE;
    }
    if (!CUTpid->IsInside(tof, pad1))
    {
        stats_pid++;
        return kTRUE;
    }
    if (CUTX1tof && !CUTX1tof->IsInside(X1pos, tof))
    {
        stats_pid2++;
        return kTRUE;
    }
    if (pad2 < 57 || pad2 > 59)
    {
        //stats_pid2++;
        //return kTRUE;
    }
    if (Y1 < -10 || Y1 > 15)
    {
        stats_y1++;
        return kTRUE;
    }
    if (Y2 < -15 || Y2 > 20)
    {
        stats_y2++;
        return kTRUE;
    }
    if (ThSCAT < -2 || ThSCAT > 2)
    {
        stats_thscat++;
        return kTRUE;
    }
    stats_spec_accepted++;

    spectrometer->Fill(Ex);
    for (size_t i = 0; i < DetectorHit.size(); ++i)
    {
        stats_si_total++;
        if (SiliconEnergy[i] < 150)
        {
            stats_si_energy_rejected++;
            continue;
        }
        raw->Fill(Ex, SiliconEnergy[i]);
        int time = (int)abs(SiliconTime[i] - tof) % 2675;
        silicontime->Fill(time);
        if(time >= tdc_gate_left && time <= tdc_gate_right)
        //if (true)
        {
            filteredDhit.push_back(DetectorHit[i]);
            filteredE.push_back(SiliconEnergy[i]);
            stats_si_accepted++;
        }
        else
        {
            stats_si_tdc_rejected++;
        }
    }

    if (filteredE.size() != filteredDhit.size())
    {
        std::cout << "Vector dimension mismatch" << std::endl;
        this->Abort("Vector dimension mismatch");
        return kFALSE;
    }

    if (filteredE.size() == 0)
    {
        return kTRUE;
    }

    for (size_t i = 0; i < filteredE.size(); ++i)
    {
        gated->Fill(Ex, filteredE[i]);
    }
    mpcty->Fill(Ex, filteredDhit.size());

    if (filteredE.size() >= 2 && filteredE.size() < 20)
    {
        for (size_t i = 0; i < filteredE.size(); ++i)
        {
            gated_coinc->Fill(Ex, filteredE[i]);
        }
    }

    return kTRUE;
}

void Li2CO3::SlaveTerminate()
{
    // The SlaveTerminate() function is called after all entries or objects
    // have been processed. When running with PROOF SlaveTerminate() is called
    // on each slave server.
    TParameter<int>* pssst = new TParameter<int>("stats_spec_total",
            stats_spec_total, '+');
    fOutput->Add(pssst);

    TParameter<int>* psssa = new TParameter<int>("stats_spec_accepted",
            stats_spec_accepted, '+');
    fOutput->Add(psssa);

    TParameter<int>* psp = new TParameter<int>("stats_pid", stats_pid, '+');
    fOutput->Add(psp);

    TParameter<int>* psp2 = new TParameter<int>("stats_pid2", stats_pid2, '+');
    fOutput->Add(psp2);

    TParameter<int>* pxc = new TParameter<int>("stats_x1_chisq",
            stats_x1_chisq, '+');
    fOutput->Add(pxc);

    TParameter<int>* px1f = new TParameter<int>("stats_x1_flag",
            stats_x1_flag, '+');
    fOutput->Add(px1f);

    TParameter<int>* px2f = new TParameter<int>("stats_x2_flag",
            stats_x2_flag, '+');
    fOutput->Add(px2f);

    TParameter<int>* pu1f = new TParameter<int>("stats_u1_flag",
            stats_u1_flag, '+');
    fOutput->Add(pu1f);

    TParameter<int>* pu2f = new TParameter<int>("stats_u2_flag",
            stats_u2_flag, '+');
    fOutput->Add(pu2f);

    TParameter<int>* py1 = new TParameter<int>("stats_y1", stats_y1, '+');
    fOutput->Add(py1);

    TParameter<int>* py2 = new TParameter<int>("stats_y2", stats_y2, '+');
    fOutput->Add(py2);

    TParameter<int>* pts = new TParameter<int>("stats_thscat",
            stats_thscat, '+');
    fOutput->Add(pts);

    TParameter<int>* ptt = new TParameter<int>("stats_si_total",
            stats_si_total, '+');
    fOutput->Add(ptt);

    TParameter<int>* pta = new TParameter<int>("stats_si_accepted",
            stats_si_accepted, '+');
    fOutput->Add(pta);

    TParameter<int>* ptr = new TParameter<int>("stats_si_tdc_rejected",
            stats_si_tdc_rejected, '+');
    fOutput->Add(ptr);

    TParameter<int>* pter = new TParameter<int>("stats_si_energy_rejected",
            stats_si_energy_rejected, '+');
    fOutput->Add(pter);
}

void Li2CO3::Terminate()
{
    // The Terminate() function is the last function to be called during
    // a query. It always runs on the client, it can be used to present
    // the results graphically or save the results to file.
    //
    gStyle->SetOptStat(0);
    TCanvas* csp = new TCanvas("c_spectrometer", "Spectrometer Position");
    spectrometer = (TH1F*)fOutput->FindObject("spectrometer");
    spectrometer->GetXaxis()->SetTitle("Spectrometer Position/mm");
    spectrometer->GetYaxis()->SetTitle("Counts");
    spectrometer->SetTitle("Spectrometer Position");
    spectrometer->Draw("");

    TCanvas* cst = new TCanvas("c_silicontime", "Silicon Time");
    silicontime = (TH1F*)fOutput->FindObject("silicontime");
    silicontime->GetXaxis()->SetTitle("Silicon Time/100ps");
    silicontime->GetYaxis()->SetTitle("Counts");
    silicontime->SetTitle("Silicon Time");
    silicontime->Draw("");
    cst->Update();
    TLine* ll = new TLine(tdc_gate_left,0,tdc_gate_left,gPad->GetUymax());
    ll->Draw();
    TLine* lr = new TLine(tdc_gate_right,0,tdc_gate_right,gPad->GetUymax());
    lr->Draw();

    TCanvas* craw = new TCanvas("c_raw", "Ungated Plot");
    raw = (TH2F*)fOutput->FindObject("raw");
    raw->GetXaxis()->SetTitle("Spectrometer Position/mm");
    raw->GetYaxis()->SetTitle("Silicon Energy/keV");
    raw->SetTitle("SiE vs X1Pos - No Gates");
    raw->Draw("colz");

    TCanvas* c1 = new TCanvas("c_gated", "Gated Plot");
    gated = (TH2F*)fOutput->FindObject("gated");
    gated->GetXaxis()->SetTitle("Spectrometer Position/mm");
    gated->GetYaxis()->SetTitle("Silicon Energy/keV");
    gated->SetTitle("SiE vs X1Pos - TDC Gated");
    gated->Draw("colz");

    TCanvas* c2 = new TCanvas("c_gated_coinc", "Coincidence Plot");
    gated_coinc = (TH2F*)fOutput->FindObject("gated_coinc");
    gated_coinc->GetXaxis()->SetTitle("Spectrometer Position/mm");
    gated_coinc->GetYaxis()->SetTitle("Silicon Energy/keV");
    gated_coinc->SetTitle("SiE vs X1Pos - TDC Gated + Coincidence");
    gated_coinc->Draw("colz");

    TCanvas* c3 = new TCanvas("c_mpcty", "Multiplicity Plot");
    mpcty = (TH2F*)fOutput->FindObject("multiplicity");
    mpcty->GetXaxis()->SetTitle("Spectrometer Position/mm");
    mpcty->GetYaxis()->SetTitle("Multiplicity");
    mpcty->SetTitle("Silicon Multiplicity vs X1Pos");
    mpcty->Draw("colz");

    stats_spec_total = ((TParameter<int>*)fOutput->
            FindObject("stats_spec_total"))->GetVal();
    stats_spec_accepted = ((TParameter<int>*)fOutput->
            FindObject("stats_spec_accepted"))->GetVal();
    stats_pid = ((TParameter<int>*)fOutput->
            FindObject("stats_pid"))->GetVal();
    stats_pid2 = ((TParameter<int>*)fOutput->
            FindObject("stats_pid2"))->GetVal();
    stats_x1_chisq = ((TParameter<int>*)fOutput->
            FindObject("stats_x1_chisq"))->GetVal();
    stats_x1_flag = ((TParameter<int>*)fOutput->
            FindObject("stats_x1_flag"))->GetVal();
    stats_x2_flag = ((TParameter<int>*)fOutput->
            FindObject("stats_x2_flag"))->GetVal();
    stats_u1_flag = ((TParameter<int>*)fOutput->
            FindObject("stats_u1_flag"))->GetVal();
    stats_u2_flag = ((TParameter<int>*)fOutput->
            FindObject("stats_u2_flag"))->GetVal();
    stats_y1 = ((TParameter<int>*)fOutput->
            FindObject("stats_y1"))->GetVal();
    stats_y2 = ((TParameter<int>*)fOutput->
            FindObject("stats_y2"))->GetVal();
    stats_thscat = ((TParameter<int>*)fOutput->
            FindObject("stats_thscat"))->GetVal();
    stats_si_total = ((TParameter<int>*)fOutput->
            FindObject("stats_si_total"))->GetVal();
    stats_si_accepted = ((TParameter<int>*)fOutput->
            FindObject("stats_si_accepted"))->GetVal();
    stats_si_tdc_rejected = ((TParameter<int>*)fOutput->
            FindObject("stats_si_tdc_rejected"))->GetVal();
    stats_si_energy_rejected = ((TParameter<int>*)fOutput->
            FindObject("stats_si_energy_rejected"))->GetVal();

    using std::cout; using std::endl;
    using std::setw; using std::setprecision;
    cout.setf(std::ios_base::right, std::ios_base::adjustfield);
    cout << "============== GATE STATISTICS ===============" << endl;
    cout << " Spectrometer Events     : " << setw(8) << stats_spec_total << endl;
    cout << " Rejected by PID         : " << setw(8) << stats_pid
        << setprecision(2) << " (" << (100.0 * stats_pid / stats_spec_total) << "%)" << endl;
    cout << " Rejected by PID2        : " << setw(8) << stats_pid2
        << setprecision(2) << " (" << (100.0 * stats_pid2 / stats_spec_total) << "%)" << endl;
    cout << " Rejected by X1 Chisq    : " << setw(8) << stats_x1_chisq
        << setprecision(2) << " (" << (100.0 * stats_x1_chisq / stats_spec_total) << "%)" << endl;
    cout << " Rejected by X1 flag     : " << setw(8) << stats_x1_flag
        << setprecision(2) << " (" << (100.0 * stats_x1_flag / stats_spec_total) << "%)" << endl;
    cout << " Rejected by X2 flag     : " << setw(8) << stats_x2_flag
        << setprecision(2) << " (" << (100.0 * stats_x2_flag / stats_spec_total) << "%)" << endl;
    cout << " Rejected by U1 flag     : " << setw(8) << stats_u1_flag
        << setprecision(2) << " (" << (100.0 * stats_u1_flag / stats_spec_total) << "%)" << endl;
    cout << " Rejected by U2 flag     : " << setw(8) << stats_u2_flag
        << setprecision(2) << " (" << (100.0 * stats_u2_flag / stats_spec_total) << "%)" << endl;
    cout << " Rejected by Y1          : " << setw(8) << stats_y1
        << setprecision(2) << " (" << (100.0 * stats_y1 / stats_spec_total) << "%)" << endl;
    cout << " Rejected by Y2          : " << setw(8) << stats_y2
        << setprecision(2) << " (" << (100.0 * stats_y2 / stats_spec_total) << "%)" << endl;
    cout << " Rejected by ThScat      : " << setw(8) << stats_thscat
        << setprecision(2) << " (" << (100.0 * stats_thscat / stats_spec_total) << "%)" << endl;
    cout << " Accepted                : " << setw(8) << stats_spec_accepted
        << setprecision(2) << " (" << (100.0 * stats_spec_accepted / stats_spec_total) << "%)" << endl;
    cout << "----------------------------------------------" << endl;
    cout << " Silicon Events          : " << setw(8) << stats_si_total << endl;
    cout << " Rejected by Energy      : " << setw(8) << stats_si_energy_rejected
        << setprecision(2) << " (" << (100.0 * stats_si_energy_rejected / stats_si_total) << "%)" << endl;
    cout << " Rejected by TDC         : " << setw(8) << stats_si_tdc_rejected
        << setprecision(2) << " (" << (100.0 * stats_si_tdc_rejected / stats_si_total) << "%)" << endl;
    cout << " Accepted                : " << setw(8) << stats_si_accepted
        << setprecision(2) << " (" << (100.0 * stats_si_accepted / stats_si_total) << "%)" << endl;
    cout << "----------------------------------------------" << endl;

    csp->cd();
    TF1* specfit = new TF1("specfit", "gaus");
    spectrometer->Fit("specfit", "QN", "", x1_oxygen_left, x1_oxygen_right);
    double a = specfit->GetParameter(0);
    double mu = specfit->GetParameter(1);
    double sigma = specfit->GetParameter(2);
    delete specfit;
    specfit = new TF1("specfit", "gaus(0)+pol1(3)");
    specfit->SetParameter(0, a);
    specfit->SetParameter(1, mu);
    specfit->SetParameter(2, sigma);
    spectrometer->Fit("specfit", "QLM", "", x1_background_l, x1_background_r);

    TCanvas* cgatedpx = new TCanvas("cgatedpx", "Gated Projection X");
    double firstx = gated->GetYaxis()->FindBin(x1_oxygen_bottom);
    double lastx = gated->GetYaxis()->FindBin(x1_oxygen_top);
    TH1D* gated_px = gated->ProjectionX("_px", firstx, lastx);
    gated_px->GetYaxis()->SetTitle("Counts");
    TF1* gatedfit = new TF1("gatedfit", "gaus");
    gated_px->Fit("gatedfit", "QN", "", x1_oxygen_left, x1_oxygen_right);
    a = gatedfit->GetParameter(0);
    mu = gatedfit->GetParameter(1);
    sigma = gatedfit->GetParameter(2);
    delete gatedfit;
    gatedfit = new TF1("gatedfit", "gaus(0)+pol1(3)");
    gatedfit->SetParameter(0, a);
    gatedfit->SetParameter(1, mu);
    gatedfit->SetParameter(2, sigma);
    gated_px->Fit("gatedfit", "QLM", "", x1_background_l, x1_background_r);
    gated_px->Draw();

    TCanvas* ccoincpx = new TCanvas("ccoincpx", "Coincidence Projection X");
    firstx = gated_coinc->GetYaxis()->FindBin(x1_oxygen_bottom);
    lastx = gated_coinc->GetYaxis()->FindBin(x1_oxygen_top);
    TH1D* coinc_px = gated_coinc->ProjectionX("_px", firstx, lastx);
    coinc_px->GetYaxis()->SetTitle("Counts");
    TF1* coincfit = new TF1("coincfit", "gaus");
    coinc_px->Fit("coincfit", "QLN", "", x1_oxygen_left, x1_oxygen_right);
    a = coincfit->GetParameter(0);
    mu = coincfit->GetParameter(1);
    sigma = coincfit->GetParameter(2);
    delete coincfit;
    coincfit = new TF1("coincfit", "gaus(0)+pol1(3)");
    coincfit->SetParameter(0, a);
    coincfit->SetParameter(1, mu);
    coincfit->SetParameter(2, sigma);
    coinc_px->Fit("coincfit", "QLM", "", x1_background_l, x1_background_r);

    coinc_px->Draw();

    TF1* specbg = new TF1("specbg", "pol1");
    specbg->FixParameter(0, specfit->GetParameter(3));
    specbg->FixParameter(1, specfit->GetParameter(4));
    csp->cd();
    specbg->SetLineColor(kGreen);
    spectrometer->Fit("specbg", "QF+", "", x1_background_l, x1_background_r);
    TF1* gatedbg = new TF1("gatedbg", "pol1");
    gatedbg->FixParameter(0, gatedfit->GetParameter(3));
    gatedbg->FixParameter(1, gatedfit->GetParameter(4));
    cgatedpx->cd();
    gatedbg->SetLineColor(kGreen);
    gated_px->Fit("gatedbg", "QF+", "", x1_background_l, x1_background_r);
    TF1* coincbg = new TF1("coincbg", "pol1");
    coincbg->FixParameter(0, coincfit->GetParameter(3));
    coincbg->FixParameter(1, coincfit->GetParameter(4));
    ccoincpx->cd();
    coincbg->SetLineColor(kGreen);
    coinc_px->Fit("coincbg", "QF+", "", x1_background_l, x1_background_r);

    TF1* specpeak = new TF1("specpeak", "gaus");
    specpeak->SetParameter(0, specfit->GetParameter(0));
    specpeak->SetParameter(1, specfit->GetParameter(1));
    specpeak->SetParameter(2, specfit->GetParameter(2));
    TF1* gatedpeak = new TF1("gatedpeak", "gaus");
    gatedpeak->SetParameter(0, gatedfit->GetParameter(0));
    gatedpeak->SetParameter(1, gatedfit->GetParameter(1));
    gatedpeak->SetParameter(2, gatedfit->GetParameter(2));
    TF1* coincpeak = new TF1("coincpeak", "gaus");
    coincpeak->SetParameter(0, coincfit->GetParameter(0));
    coincpeak->SetParameter(1, coincfit->GetParameter(1));
    coincpeak->SetParameter(2, coincfit->GetParameter(2));

    int binLeft = spectrometer->GetXaxis()->FindBin(x1_oxygen_left);
    int binRight = spectrometer->GetXaxis()->FindBin(x1_oxygen_right);
    double spec_b = specbg->Integral(x1_oxygen_left, x1_oxygen_right) / spectrometer->GetBinWidth(binLeft) *  0.259295;
    double spec_i = spectrometer->Integral(binLeft, binRight) * 0.259295;
    double spec_p = spec_i - spec_b;
    double gated_b = gatedbg->Integral(x1_oxygen_left, x1_oxygen_right) / gated_px->GetBinWidth(binLeft);
    double gated_i = gated_px->Integral(binLeft, binRight);
    double gated_p = gated_i - gated_b;
    double coinc_b = coincbg->Integral(x1_oxygen_left, x1_oxygen_right) / (coinc_px->GetBinWidth(binLeft) * 2);
    double coinc_i = coinc_px->Integral(binLeft, binRight) / 2;
    double coinc_p = coinc_i - coinc_b;

    cout << endl;
    cout << "============== PEAK STATISTICS ===============" << endl;
    cout.setf(cout.fixed, cout.floatfield);
    cout << setprecision(2);
    cout << "Total spectrometer counts: " << setw(8) << spec_i << endl;
    cout << "Spectrometer background  : " << setw(8) << spec_b << endl;
    cout << "Spectrometer peak        : " << setw(8) << spec_p
        << " (" << (spec_p * 100 / spec_i ) << "%)" << endl;
    cout << "Total single counts      : " << setw(8) << gated_i << endl;
    cout << "Single background        : " << setw(8) << gated_b << endl;
    cout << "Single peak              : " << setw(8) << gated_p
        << " (" << (gated_p * 100 / gated_i ) << "%)" << endl;
    cout << "Total double counts      : " << setw(8) << coinc_i << endl;
    cout << "Double background        : " << setw(8) << coinc_b << endl;
    cout << "Double peak              : " << setw(8) << coinc_p
        << " (" << (coinc_p * 100 / coinc_i ) << "%)" << endl;
    cout << "----------------------------------------------" << endl;
    cout << "Single efficiency        : " << setw(8) <<
        (gated_p * 100.0 / spec_p) << "%" << endl;
    cout << "Double efficiency        : " << setw(8) <<
        (coinc_p * 100.0 / spec_p) << "%" << endl;
    cout << "----------------------------------------------" << endl;

    delete specfit;
    delete gatedfit;
}

