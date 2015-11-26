#define Carbon_cxx
// The class definition in Carbon.h has been generated automatically
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
// Root > T->Process("Carbon.C")
// Root > T->Process("Carbon.C","some options")
// Root > T->Process("Carbon.C+")
//

#include "Carbon.h"

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

#include "config.h"

using std::size_t;

void Carbon::Begin(TTree * /*tree*/)
{
    // The Begin() function is called at the start of the query.
    // When running with PROOF Begin() is only called on the client.
    // The tree argument is deprecated (on PROOF 0 is passed).
    //
    AnalysisConfig& config = AnalysisConfig::Instance();

    energy_gate = config.UseEnergyGate();
    energy_min = config.EnergyMin();
    tdc_gate = config.UseTDCGate();
    tdc_gate_left = config.TDCGateLeft();
    tdc_gate_right = config.TDCGateRight();
    x1_carbon_left = config.ExCarbonLeft();
    x1_carbon_right = config.ExCarbonRight();
    x1_carbon_top = config.ExCarbonTop();
    x1_carbon_bottom = config.ExCarbonBottom();
    x1_background_l = config.ExCarbonBackgroundLeft();
    x1_background_r = config.ExCarbonBackgroundRight();
    efficiency = config.PairEfficiency();
    embedPID = false;

    CUTpid = 0;
    CUTX1tof = 0;
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
    if (config.UseX1tofPID())
    {
        TFile* pidfile = new TFile(
                config.X1tofPIDfile() + TString(".root"), "OLD"
                );
        CUTX1tof = (TCutG*)pidfile->Get("CUTX1tof");
        delete pidfile;
    }

    if (fInput)
    {
        if (CUTpid)
            fInput->Add(CUTpid);
        if (CUTX1tof)
            fInput->Add(CUTX1tof);
        fInput->Add(
                new TParameter<bool>("embedPID", embedPID)
                );
        fInput->Add(
                new TParameter<bool>("energy_gate", energy_gate)
                );
        fInput->Add(
                new TParameter<double>("energy_min", energy_min)
                );
        fInput->Add(
               new TParameter<bool>("tdc_gate", tdc_gate)
               );
        fInput->Add(
                new TParameter<int>("tdc_gate_left", tdc_gate_left)
                );
        fInput->Add(
                new TParameter<int>("tdc_gate_right", tdc_gate_right)
                );
        fInput->Add(
                new TParameter<double>("x1_carbon_left", x1_carbon_left)
                );
        fInput->Add(
                new TParameter<double>("x1_carbon_right", x1_carbon_right)
                );
        fInput->Add(
                new TParameter<double>("x1_carbon_top", x1_carbon_top)
                );
        fInput->Add(
                new TParameter<double>("x1_carbon_bottom", x1_carbon_bottom)
                );
        fInput->Add(
                new TParameter<double>("x1_background_l", x1_background_l)
                );
        fInput->Add(
                new TParameter<double>("x1_background_r", x1_background_r)
                );
        fInput->Add(
                new TParameter<double>("efficiency", efficiency)
                );
    }
}

void Carbon::SlaveBegin(TTree * /*tree*/)
{
    // The SlaveBegin() function is called after the Begin() function.
    // When running with PROOF SlaveBegin() is called on each slave server.
    // The tree argument is deprecated (on PROOF 0 is passed).

    TString option = GetOption();

    spectrometer = new TH1F("spectrometer", "", 800, 4, 16);
    silicontime = new TH1F("silicontime", "", 1500, -4500, 4500);
    raw = new TH2F("raw", "", 800, 4, 16, 1000, 0, 10000);
    gated = new TH2F("gated", "", 800, 4, 16, 1000, 0, 10000);
    gated_coinc = new TH2F("gated_coinc", "", 800, 4, 16, 1000, 0, 10000);
    mpcty = new TH2F("multiplicity", "", 800, 4, 16, 224, 0, 224);
    fOutput->Add(spectrometer);
    fOutput->Add(silicontime);
    fOutput->Add(raw);
    fOutput->Add(gated);
    fOutput->Add(gated_coinc);
    fOutput->Add(mpcty);

    if (fInput)
    {
        embedPID = ((TParameter<bool>*)fInput->FindObject("embedPID"))->GetVal();
        energy_gate = ((TParameter<bool>*)fInput->FindObject("energy_gate"))->GetVal();
        energy_min = ((TParameter<double>*)fInput->FindObject("energy_min"))->GetVal();
        tdc_gate = ((TParameter<bool>*)fInput->FindObject("tdc_gate"))->GetVal();
        tdc_gate_left = ((TParameter<int>*)fInput->FindObject("tdc_gate_left"))->GetVal();
        tdc_gate_right = ((TParameter<int>*)fInput->FindObject("tdc_gate_right"))->GetVal();
        x1_carbon_left = ((TParameter<double>*)fInput->FindObject("x1_carbon_left"))->GetVal();
        x1_carbon_right = ((TParameter<double>*)fInput->FindObject("x1_carbon_right"))->GetVal();
        x1_carbon_top = ((TParameter<double>*)fInput->FindObject("x1_carbon_top"))->GetVal();
        x1_carbon_bottom = ((TParameter<double>*)fInput->FindObject("x1_carbon_bottom"))->GetVal();
        x1_background_l = ((TParameter<double>*)fInput->FindObject("x1_background_l"))->GetVal();
        x1_background_r = ((TParameter<double>*)fInput->FindObject("x1_background_r"))->GetVal();
        efficiency = ((TParameter<double>*)fInput->FindObject("efficiency"))->GetVal();
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

Bool_t Carbon::Process(Long64_t entry)
{
    // The Process() function is called for each entry in the tree (or possibly
    // keyed object in the case of PROOF) to be processed. The entry argument
    // specifies which entry in the currently loaded tree is to be processed.
    // It can be passed to either Carbon::GetEntry() or TBranch::GetEntry()
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
    b_SiliconInfo_EnergyBack->GetEntry(entry);
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
    b_t_PIDgood->GetEntry(entry);

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
    if (CUTpid && !CUTpid->IsInside(tof, pad1))
    {
        stats_pid++;
        return kTRUE;
    }
    if (embedPID && !PIDgood)
    {
        stats_pid++;
        return kTRUE;
    }
    if (CUTX1tof && !CUTX1tof->IsInside(X1pos, tof))
    {
        stats_pid2++;
        return kTRUE;
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
        raw->Fill(Ex, SiliconEnergy[i]);
        if (energy_gate && SiliconEnergy[i] < energy_min)
        {
            stats_si_energy_rejected++;
            continue;
        }
        if (energy_gate && EnergyBack[i] < energy_min)
        {
            stats_si_energy_rejected++;
            continue;
        }
        int time = SiliconTime[i] - tof;
        silicontime->Fill(time);
        if(!tdc_gate || (time >= tdc_gate_left && time <= tdc_gate_right))
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

    std::vector<int> hpd(8);

    for (size_t i = 0; i < filteredE.size(); ++i)
    {
        gated->Fill(Ex, filteredE[i]);
        hpd[filteredDhit[i]-1] += 1;
    }
    int dets = 0;
    for (size_t i = 0; i < hpd.size(); ++i)
    {
        if (hpd[i]) {
            dets++;
        }
    }

    mpcty->Fill(Ex, filteredDhit.size());
    //mpcty->Fill(Ex, dets);

    if (filteredE.size() >= 2 && filteredE.size() < 20)
    {
        for (size_t i = 0; i < filteredE.size(); ++i)
        {
            gated_coinc->Fill(Ex, filteredE[i]);
        }
    }

    return kTRUE;
}

void Carbon::SlaveTerminate()
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

void Carbon::Terminate()
{
    // The Terminate() function is the last function to be called during
    // a query. It always runs on the client, it can be used to present
    // the results graphically or save the results to file.
    //
    gStyle->SetOptStat(0);
    TCanvas* csp = new TCanvas("c_spectrometer", "Excitation Energy");
    spectrometer = (TH1F*)fOutput->FindObject("spectrometer");
    spectrometer->GetXaxis()->SetTitle("Excitation Energy/MeV");
    spectrometer->GetYaxis()->SetTitle("Counts");
    spectrometer->SetTitle("Excitation Energy");
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
    raw->GetXaxis()->SetTitle("Excitation Energy/MeV");
    raw->GetYaxis()->SetTitle("Silicon Energy/keV");
    raw->SetTitle("Silicon Energy vs Excitation Energy - No Silicon Gates");
    raw->Draw("colz");

    TCanvas* c1 = new TCanvas("c_gated", "Gated Plot");
    gated = (TH2F*)fOutput->FindObject("gated");
    gated->GetXaxis()->SetTitle("Excitation Energy/MeV");
    gated->GetYaxis()->SetTitle("Silicon Energy/keV");
    gated->SetTitle("Silicon Energy vs Excitation Energy - TDC Gated");
    gated->Draw("colz");

    TCanvas* c2 = new TCanvas("c_gated_coinc", "Coincidence Plot");
    gated_coinc = (TH2F*)fOutput->FindObject("gated_coinc");
    gated_coinc->GetXaxis()->SetTitle("Excitation Energy/MeV");
    gated_coinc->GetYaxis()->SetTitle("Silicon Energy/keV");
    gated_coinc->SetTitle("Silicon Energy vs Excitation Energy - TDC Gated + Coincidence");
    gated_coinc->Draw("colz");

    TCanvas* c3 = new TCanvas("c_mpcty", "Multiplicity Plot");
    mpcty = (TH2F*)fOutput->FindObject("multiplicity");
    mpcty->GetXaxis()->SetTitle("Excitation Energy/MeV");
    mpcty->GetYaxis()->SetTitle("Multiplicity");
    mpcty->SetTitle("Silicon Multiplicity vs Excitation Energy");
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
    cout << " Spectrometer Events     : " << setw(10) << stats_spec_total << endl;
    cout << " Rejected by PID         : " << setw(10) << stats_pid
        << setprecision(2) << " (" << (100.0 * stats_pid / stats_spec_total) << "%)" << endl;
    cout << " Rejected by PID2        : " << setw(10) << stats_pid2
        << setprecision(2) << " (" << (100.0 * stats_pid2 / stats_spec_total) << "%)" << endl;
    cout << " Rejected by X1 Chisq    : " << setw(10) << stats_x1_chisq
        << setprecision(2) << " (" << (100.0 * stats_x1_chisq / stats_spec_total) << "%)" << endl;
    cout << " Rejected by X1 flag     : " << setw(10) << stats_x1_flag
        << setprecision(2) << " (" << (100.0 * stats_x1_flag / stats_spec_total) << "%)" << endl;
    cout << " Rejected by X2 flag     : " << setw(10) << stats_x2_flag
        << setprecision(2) << " (" << (100.0 * stats_x2_flag / stats_spec_total) << "%)" << endl;
    cout << " Rejected by U1 flag     : " << setw(10) << stats_u1_flag
        << setprecision(2) << " (" << (100.0 * stats_u1_flag / stats_spec_total) << "%)" << endl;
    cout << " Rejected by U2 flag     : " << setw(10) << stats_u2_flag
        << setprecision(2) << " (" << (100.0 * stats_u2_flag / stats_spec_total) << "%)" << endl;
    cout << " Rejected by Y1          : " << setw(10) << stats_y1
        << setprecision(2) << " (" << (100.0 * stats_y1 / stats_spec_total) << "%)" << endl;
    cout << " Rejected by Y2          : " << setw(10) << stats_y2
        << setprecision(2) << " (" << (100.0 * stats_y2 / stats_spec_total) << "%)" << endl;
    cout << " Rejected by ThScat      : " << setw(10) << stats_thscat
        << setprecision(2) << " (" << (100.0 * stats_thscat / stats_spec_total) << "%)" << endl;
    cout << " Accepted                : " << setw(10) << stats_spec_accepted
        << setprecision(2) << " (" << (100.0 * stats_spec_accepted / stats_spec_total) << "%)" << endl;
    cout << "----------------------------------------------" << endl;
    cout << " Silicon Events          : " << setw(10) << stats_si_total << endl;
    cout << " Rejected by Energy      : " << setw(10) << stats_si_energy_rejected
        << setprecision(2) << " (" << (100.0 * stats_si_energy_rejected / stats_si_total) << "%)" << endl;
    cout << " Rejected by TDC         : " << setw(10) << stats_si_tdc_rejected
        << setprecision(2) << " (" << (100.0 * stats_si_tdc_rejected / stats_si_total) << "%)" << endl;
    cout << " Accepted                : " << setw(10) << stats_si_accepted
        << setprecision(2) << " (" << (100.0 * stats_si_accepted / stats_si_total) << "%)" << endl;
    cout << "----------------------------------------------" << endl;

    csp->cd();
    TF1* specfit = new TF1("specfit", "gaus");
    spectrometer->Fit("specfit", "QN", "", x1_carbon_left, x1_carbon_right);
    double a = specfit->GetParameter(0);
    double mu = specfit->GetParameter(1);
    double sigma = specfit->GetParameter(2);
    delete specfit;
    specfit = new TF1("specfit", "gaus(0)+pol1(3)");
    specfit->SetParameter(0, a);
    specfit->SetParameter(1, mu);
    specfit->SetParameter(2, sigma);
    spectrometer->Fit("specfit", "FQLM", "", x1_background_l, x1_background_r);

    TCanvas* cgatedpx = new TCanvas("cgatedpx", "Gated Projection X");
    double firstx = gated->GetYaxis()->FindBin(x1_carbon_bottom);
    double lastx = gated->GetYaxis()->FindBin(x1_carbon_top);
    TH1D* gated_px = gated->ProjectionX("_px", firstx, lastx);
    gated_px->GetYaxis()->SetTitle("Counts");
    TF1* gatedfit = new TF1("gatedfit", "gaus");
    gated_px->Fit("gatedfit", "QN", "", x1_carbon_left, x1_carbon_right);
    a = gatedfit->GetParameter(0);
    mu = gatedfit->GetParameter(1);
    sigma = gatedfit->GetParameter(2);
    delete gatedfit;
    gatedfit = new TF1("gatedfit", "gaus(0)+pol1(3)");
    gatedfit->SetParameter(0, a);
    gatedfit->SetParameter(1, mu);
    gatedfit->SetParameter(2, sigma);
    gated_px->Fit("gatedfit", "QM", "", x1_background_l, x1_background_r);
    gated_px->Draw();

    TCanvas* ccoincpx = new TCanvas("ccoincpx", "Coincidence Projection X");
    firstx = gated_coinc->GetYaxis()->FindBin(x1_carbon_bottom);
    lastx = gated_coinc->GetYaxis()->FindBin(x1_carbon_top);
    TH1D* coinc_px = gated_coinc->ProjectionX("_px", firstx, lastx);
    coinc_px->GetYaxis()->SetTitle("Counts");
    TF1* coincfit = new TF1("coincfit", "gaus");
    coinc_px->Fit("coincfit", "QN", "", x1_carbon_left, x1_carbon_right);
    a = coincfit->GetParameter(0);
    mu = coincfit->GetParameter(1);
    sigma = coincfit->GetParameter(2);
    delete coincfit;
    coincfit = new TF1("coincfit", "gaus(0)+pol1(3)");
    coincfit->SetParameter(0, a);
    coincfit->SetParameter(1, mu);
    coincfit->SetParameter(2, sigma);
    coinc_px->Fit("coincfit", "QM", "", x1_background_l, x1_background_r);

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

    int binLeft = spectrometer->GetXaxis()->FindBin(x1_carbon_left);
    int binRight = spectrometer->GetXaxis()->FindBin(x1_carbon_right);

    double spec_b = specbg->Integral(x1_carbon_left, x1_carbon_right) / spectrometer->GetBinWidth(binLeft);
    double spec_i = spectrometer->Integral(binLeft, binRight);
    double spec_p = spec_i - spec_b;
    double gated_b = gatedbg->Integral(x1_carbon_left, x1_carbon_right) / gated_px->GetBinWidth(binLeft);
    double gated_i = gated_px->Integral(binLeft, binRight);
    double gated_p = gated_i - gated_b;
    double coinc_b = coincbg->Integral(x1_carbon_left, x1_carbon_right) / (coinc_px->GetBinWidth(binLeft) * 2);
    double coinc_i = coinc_px->Integral(binLeft, binRight) / 2;
    double coinc_p = coinc_i - coinc_b;

    cout << endl;
    cout << "============== PEAK STATISTICS ===============" << endl;
    cout.setf(cout.fixed, cout.floatfield);
    cout << setprecision(2);
    cout << "Total spectrometer counts: " << setw(10) << spec_i << endl;
    cout << "Spectrometer background  : " << setw(10) << spec_b << endl;
    cout << "Spectrometer peak        : " << setw(10) << spec_p
        << " (" << (spec_p * 100 / spec_i ) << "%)" << endl;
    cout << "Total single counts      : " << setw(10) << gated_i << endl;
    cout << "Single background        : " << setw(10) << gated_b << endl;
    cout << "Single peak              : " << setw(10) << gated_p
        << " (" << (gated_p * 100 / gated_i ) << "%)" << endl;
    cout << "Total double counts      : " << setw(10) << coinc_i << endl;
    cout << "Double background        : " << setw(10) << coinc_b << endl;
    cout << "Double peak              : " << setw(10) << coinc_p
        << " (" << (coinc_p * 100 / coinc_i ) << "%)" << endl;
    cout << "----------------------------------------------" << endl;
    cout << "Double efficiency        : " << setw(10) <<
        (efficiency) << "%" << endl;
    cout << "Single ratio             : " << setw(10) << std::scientific <<
        (gated_p / spec_p) << endl;
    cout << "Double ratio             : " << setw(10) << std::scientific <<
        (coinc_p / spec_p) << endl;
    cout << "Branching ratio          : " << setw(10) << std::scientific <<
        (coinc_p / spec_p / (efficiency / 100)) << endl;
    cout << "----------------------------------------------" << endl;
}

