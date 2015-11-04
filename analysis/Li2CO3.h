//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Feb  3 16:30:27 2015 by ROOT version 5.34/22
// from TTree DATA/K600 data
// found on file: run00100.root
//////////////////////////////////////////////////////////

#ifndef Li2CO3_h
#define Li2CO3_h

#include <TROOT.h>
#include <TChain.h>
#include <TCutG.h>
#include <TFile.h>
#include <TH2.h>
#include <TH2F.h>
#include <TSelector.h>
#include <vector>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class Li2CO3 : public TSelector {
    public :
        TTree          *fChain;   //!pointer to the analyzed TTree or TChain

        // Input data (from client)
        TCutG* CUTpid;
        TCutG* CUTX1tof;
        bool energy_gate;
        double energy_min;
        bool tdc_gate;
        int tdc_gate_left;
        int tdc_gate_right;
        double x1_oxygen_left;
        double x1_oxygen_right;
        double x1_oxygen_top;
        double x1_oxygen_bottom;
        double x1_background_l;
        double x1_background_r;

        // Output data
        TH1F* spectrometer;
        TH1F* silicontime;
        TH2F* raw;
        TH2F* gated;
        TH2F* gated_coinc;
        TH2F* mpcty;

        // Statistics
        int stats_spec_total;
        int stats_spec_accepted;
        int stats_pid;
        int stats_pid2;
        int stats_x1_chisq;
        int stats_x1_flag;
        int stats_x2_flag;
        int stats_u1_flag;
        int stats_u2_flag;
        int stats_y1;
        int stats_y2;
        int stats_thscat;

        int stats_si_total;
        int stats_si_accepted;
        int stats_si_tdc_rejected;
        int stats_si_energy_rejected;

        // Declaration of leaf types
        Int_t           runtime;
        Int_t           evtcounter;
        Int_t           tdcsperevent;
        Int_t           tof;
        Int_t           toftdc2;
        Int_t           toftdc3;
        Int_t           toftdc4;
        Int_t           toftdc5;
        Int_t           toftdc6;
        Int_t           toftdc7;
        Int_t           k600;
        Double_t        pad1;
        Double_t        pad2;
        Double_t        pad1hiP;
        Double_t        pad1lowP;
        Double_t        pad2hiP;
        Double_t        pad2lowP;
        Double_t        pad1hiPT;
        Double_t        pad1lowPT;
        Double_t        pad2hiPT;
        Double_t        pad2lowPT;
        Double_t        Ex;
        Double_t        X1pos;
        Double_t        X1th;
        Int_t           X1flag;
        Double_t        X1chisq;
        Double_t        X1res0;
        Double_t        X1res1;
        Int_t           X1hits;
        Double_t        X1effID;
        Double_t        X1effall;
        Double_t        X1effdt;
        Double_t        X1effgroup;
        Double_t        X1effgood;
        Double_t        U1pos;
        Double_t        U1th;
        Int_t           U1flag;
        Double_t        U1chisq;
        Double_t        U1res0;
        Double_t        U1res1;
        Double_t        U1effID;
        Int_t           U1hits;
        Double_t        U1effall;
        Double_t        U1effdt;
        Double_t        U1effgroup;
        Double_t        U1effgood;
        Double_t        X2pos;
        Double_t        X2th;
        Int_t           X2flag;
        Double_t        X2chisq;
        Double_t        X2res0;
        Double_t        X2res1;
        Double_t        X2effID;
        Int_t           X2hits;
        Double_t        X2effall;
        Double_t        X2effdt;
        Double_t        X2effgroup;
        Double_t        X2effgood;
        Double_t        U2pos;
        Double_t        U2th;
        Int_t           U2flag;
        Double_t        U2chisq;
        Double_t        U2res0;
        Double_t        U2res1;
        Double_t        U2effID;
        Int_t           U2hits;
        Double_t        U2effall;
        Double_t        U2effdt;
        Double_t        U2effgroup;
        Double_t        U2effgood;
        Double_t        ThSCAT;
        Double_t        ThFP;
        Double_t        PhiFP;
        Double_t        ThFPx;
        Double_t        Y1;
        Double_t        Y2;
        Int_t           pulser;
        Double_t        X1posC;
        //SiliconData     *SiliconInfo;
        Int_t           SiliconHits;
        std::vector<double>  SiliconEnergy;
        std::vector<double>  SiliconTheta;
        std::vector<double>  SiliconPhi;
        std::vector<double>  SiliconTime;
        std::vector<int>     DetectorHit;
        std::vector<int>     ADCChannelFront;
        std::vector<int>     ADCChannelBack;
        std::vector<int>     TDCChannelFront;
        std::vector<int>     TDCChannelBack;
        std::vector<int>     ADCValueFront;
        std::vector<int>     ADCValueBack;
        std::vector<int>     TDCValueFront;
        std::vector<int>     TDCValueBack;
        std::vector<int>     StripFront;
        std::vector<int>     StripBack;
        std::vector<double>  EnergyFront;
        std::vector<double>  EnergyBack;
        std::vector<int>     TDCChannelMultiplicity;
        //RawData         *RawInfo;
        Int_t           ADCValues[128];
        Int_t           TDCChannels[768];
        Int_t           TDCValues[768];

        // List of branches
        TBranch        *b_t_runtime;   //!
        TBranch        *b_t_evtcounter;   //!
        TBranch        *b_t_tdcsperevent;   //!
        TBranch        *b_t_tof;   //!
        TBranch        *b_t_toftdc2;   //!
        TBranch        *b_t_toftdc3;   //!
        TBranch        *b_t_toftdc4;   //!
        TBranch        *b_t_toftdc5;   //!
        TBranch        *b_t_toftdc6;   //!
        TBranch        *b_t_toftdc7;   //!
        TBranch        *b_t_k600;   //!
        TBranch        *b_t_pad1;   //!
        TBranch        *b_t_pad2;   //!
        TBranch        *b_t_pad1hiP;   //!
        TBranch        *b_t_pad1lowP;   //!
        TBranch        *b_t_pad2hiP;   //!
        TBranch        *b_t_pad2lowP;   //!
        TBranch        *b_t_pad1hiPT;   //!
        TBranch        *b_t_pad1lowPT;   //!
        TBranch        *b_t_pad2hiPT;   //!
        TBranch        *b_t_pad2lowPT;   //!
        TBranch        *b_t_Ex;
        TBranch        *b_t_X1pos;   //!
        TBranch        *b_t_X1th;   //!
        TBranch        *b_t_X1flag;   //!
        TBranch        *b_t_X1chisq;   //!
        TBranch        *b_t_X1res0;   //!
        TBranch        *b_t_X1res1;   //!
        TBranch        *b_t_X1hits;   //!
        TBranch        *b_t_X1effID;   //!
        TBranch        *b_t_X1effall;   //!
        TBranch        *b_t_X1effdt;   //!
        TBranch        *b_t_X1effgroup;   //!
        TBranch        *b_t_X1effgood;   //!
        TBranch        *b_t_U1pos;   //!
        TBranch        *b_t_U1th;   //!
        TBranch        *b_t_U1flag;   //!
        TBranch        *b_t_U1chisq;   //!
        TBranch        *b_t_U1res0;   //!
        TBranch        *b_t_U1res1;   //!
        TBranch        *b_t_U1effID;   //!
        TBranch        *b_t_U1hits;   //!
        TBranch        *b_t_U1effall;   //!
        TBranch        *b_t_U1effdt;   //!
        TBranch        *b_t_U1effgroup;   //!
        TBranch        *b_t_U1effgood;   //!
        TBranch        *b_t_X2pos;   //!
        TBranch        *b_t_X2th;   //!
        TBranch        *b_t_X2flag;   //!
        TBranch        *b_t_X2chisq;   //!
        TBranch        *b_t_X2res0;   //!
        TBranch        *b_t_X2res1;   //!
        TBranch        *b_t_X2effID;   //!
        TBranch        *b_t_X2hits;   //!
        TBranch        *b_t_X2effall;   //!
        TBranch        *b_t_X2effdt;   //!
        TBranch        *b_t_X2effgroup;   //!
        TBranch        *b_t_X2effgood;   //!
        TBranch        *b_t_U2pos;   //!
        TBranch        *b_t_U2th;   //!
        TBranch        *b_t_U2flag;   //!
        TBranch        *b_t_U2chisq;   //!
        TBranch        *b_t_U2res0;   //!
        TBranch        *b_t_U2res1;   //!
        TBranch        *b_t_U2effID;   //!
        TBranch        *b_t_U2hits;   //!
        TBranch        *b_t_U2effall;   //!
        TBranch        *b_t_U2effdt;   //!
        TBranch        *b_t_U2effgroup;   //!
        TBranch        *b_t_U2effgood;   //!
        TBranch        *b_t_ThSCAT;   //!
        TBranch        *b_t_ThFP;   //!
        TBranch        *b_t_PhiFP;   //!
        TBranch        *b_t_ThFPx;   //!
        TBranch        *b_t_Y1;   //!
        TBranch        *b_t_Y2;   //!
        TBranch        *b_t_pulser;   //!
        TBranch        *b_t_X1posC;   //!
        TBranch        *b_SiliconInfo_SiliconHits;   //!
        TBranch        *b_SiliconInfo_SiliconEnergy;   //!
        TBranch        *b_SiliconInfo_SiliconTheta;   //!
        TBranch        *b_SiliconInfo_SiliconPhi;   //!
        TBranch        *b_SiliconInfo_SiliconTime;   //!
        TBranch        *b_SiliconInfo_DetectorHit;   //!
        TBranch        *b_SiliconInfo_ADCChannelFront;   //!
        TBranch        *b_SiliconInfo_ADCChannelBack;   //!
        TBranch        *b_SiliconInfo_TDCChannelFront;   //!
        TBranch        *b_SiliconInfo_TDCChannelBack;   //!
        TBranch        *b_SiliconInfo_ADCValueFront;   //!
        TBranch        *b_SiliconInfo_ADCValueBack;   //!
        TBranch        *b_SiliconInfo_TDCValueFront;   //!
        TBranch        *b_SiliconInfo_TDCValueBack;   //!
        TBranch        *b_SiliconInfo_StripFront;   //!
        TBranch        *b_SiliconInfo_StripBack;   //!
        TBranch        *b_SiliconInfo_EnergyFront;   //!
        TBranch        *b_SiliconInfo_EnergyBack;   //!
        TBranch        *b_SiliconInfo_TDCChannelMultiplicity;   //!
        TBranch        *b_RawInfo_ADCValues;   //!
        TBranch        *b_RawInfo_TDCChannels;   //!
        TBranch        *b_RawInfo_TDCValues;   //!

        Li2CO3(TTree * /*tree*/ =0) : fChain(0), raw(0), gated(0), gated_coinc(0),
        mpcty(0), silicontime(0), spectrometer(0)  { }
        virtual ~Li2CO3() { }
        virtual Int_t   Version() const { return 2; }
        virtual void    Begin(TTree *tree);
        virtual void    SlaveBegin(TTree *tree);
        virtual void    Init(TTree *tree);
        virtual Bool_t  Notify();
        virtual Bool_t  Process(Long64_t entry);
        virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
        virtual void    SetOption(const char *option) { fOption = option; }
        virtual void    SetObject(TObject *obj) { fObject = obj; }
        virtual void    SetInputList(TList *input) { fInput = input; }
        virtual TList  *GetOutputList() const { return fOutput; }
        virtual void    SlaveTerminate();
        virtual void    Terminate();

        ClassDef(Li2CO3,0);
};

#endif

#ifdef Li2CO3_cxx
void Li2CO3::Init(TTree *tree)
{
    // The Init() function is called when the selector needs to initialize
    // a new tree or chain. Typically here the branch addresses and branch
    // pointers of the tree will be set.
    // It is normally not necessary to make changes to the generated
    // code, but the routine can be extended by the user if needed.
    // Init() will be called many times when running on PROOF
    // (once per file to be processed).

    // Set branch addresses and branch pointers
    if (!tree) return;
    fChain = tree;
    fChain->SetMakeClass(1);

    fChain->SetBranchAddress("runtime", &runtime, &b_t_runtime);
    fChain->SetBranchAddress("evtcounter", &evtcounter, &b_t_evtcounter);
    fChain->SetBranchAddress("tdcsperevent", &tdcsperevent, &b_t_tdcsperevent);
    fChain->SetBranchAddress("tof", &tof, &b_t_tof);
    fChain->SetBranchAddress("toftdc2", &toftdc2, &b_t_toftdc2);
    fChain->SetBranchAddress("toftdc3", &toftdc3, &b_t_toftdc3);
    fChain->SetBranchAddress("toftdc4", &toftdc4, &b_t_toftdc4);
    fChain->SetBranchAddress("toftdc5", &toftdc5, &b_t_toftdc5);
    fChain->SetBranchAddress("toftdc6", &toftdc6, &b_t_toftdc6);
    fChain->SetBranchAddress("toftdc7", &toftdc7, &b_t_toftdc7);
    fChain->SetBranchAddress("k600", &k600, &b_t_k600);
    fChain->SetBranchAddress("pad1", &pad1, &b_t_pad1);
    fChain->SetBranchAddress("pad2", &pad2, &b_t_pad2);
    fChain->SetBranchAddress("pad1hiP", &pad1hiP, &b_t_pad1hiP);
    fChain->SetBranchAddress("pad1lowP", &pad1lowP, &b_t_pad1lowP);
    fChain->SetBranchAddress("pad2hiP", &pad2hiP, &b_t_pad2hiP);
    fChain->SetBranchAddress("pad2lowP", &pad2lowP, &b_t_pad2lowP);
    fChain->SetBranchAddress("pad1hiPT", &pad1hiPT, &b_t_pad1hiPT);
    fChain->SetBranchAddress("pad1lowPT", &pad1lowPT, &b_t_pad1lowPT);
    fChain->SetBranchAddress("pad2hiPT", &pad2hiPT, &b_t_pad2hiPT);
    fChain->SetBranchAddress("pad2lowPT", &pad2lowPT, &b_t_pad2lowPT);
    fChain->SetBranchAddress("Ex", &Ex, &b_t_Ex);
    fChain->SetBranchAddress("X1pos", &X1pos, &b_t_X1pos);
    fChain->SetBranchAddress("X1th", &X1th, &b_t_X1th);
    fChain->SetBranchAddress("X1flag", &X1flag, &b_t_X1flag);
    fChain->SetBranchAddress("X1chisq", &X1chisq, &b_t_X1chisq);
    fChain->SetBranchAddress("X1res0", &X1res0, &b_t_X1res0);
    fChain->SetBranchAddress("X1res1", &X1res1, &b_t_X1res1);
    fChain->SetBranchAddress("X1hits", &X1hits, &b_t_X1hits);
    fChain->SetBranchAddress("X1effID", &X1effID, &b_t_X1effID);
    fChain->SetBranchAddress("X1effall", &X1effall, &b_t_X1effall);
    fChain->SetBranchAddress("X1effdt", &X1effdt, &b_t_X1effdt);
    fChain->SetBranchAddress("X1effgroup", &X1effgroup, &b_t_X1effgroup);
    fChain->SetBranchAddress("X1effgood", &X1effgood, &b_t_X1effgood);
    fChain->SetBranchAddress("U1pos", &U1pos, &b_t_U1pos);
    fChain->SetBranchAddress("U1th", &U1th, &b_t_U1th);
    fChain->SetBranchAddress("U1flag", &U1flag, &b_t_U1flag);
    fChain->SetBranchAddress("U1chisq", &U1chisq, &b_t_U1chisq);
    fChain->SetBranchAddress("U1res0", &U1res0, &b_t_U1res0);
    fChain->SetBranchAddress("U1res1", &U1res1, &b_t_U1res1);
    fChain->SetBranchAddress("U1effID", &U1effID, &b_t_U1effID);
    fChain->SetBranchAddress("U1hits", &U1hits, &b_t_U1hits);
    fChain->SetBranchAddress("U1effall", &U1effall, &b_t_U1effall);
    fChain->SetBranchAddress("U1effdt", &U1effdt, &b_t_U1effdt);
    fChain->SetBranchAddress("U1effgroup", &U1effgroup, &b_t_U1effgroup);
    fChain->SetBranchAddress("U1effgood", &U1effgood, &b_t_U1effgood);
    fChain->SetBranchAddress("X2pos", &X2pos, &b_t_X2pos);
    fChain->SetBranchAddress("X2th", &X2th, &b_t_X2th);
    fChain->SetBranchAddress("X2flag", &X2flag, &b_t_X2flag);
    fChain->SetBranchAddress("X2chisq", &X2chisq, &b_t_X2chisq);
    fChain->SetBranchAddress("X2res0", &X2res0, &b_t_X2res0);
    fChain->SetBranchAddress("X2res1", &X2res1, &b_t_X2res1);
    fChain->SetBranchAddress("X2effID", &X2effID, &b_t_X2effID);
    fChain->SetBranchAddress("X2hits", &X2hits, &b_t_X2hits);
    fChain->SetBranchAddress("X2effall", &X2effall, &b_t_X2effall);
    fChain->SetBranchAddress("X2effdt", &X2effdt, &b_t_X2effdt);
    fChain->SetBranchAddress("X2effgroup", &X2effgroup, &b_t_X2effgroup);
    fChain->SetBranchAddress("X2effgood", &X2effgood, &b_t_X2effgood);
    fChain->SetBranchAddress("U2pos", &U2pos, &b_t_U2pos);
    fChain->SetBranchAddress("U2th", &U2th, &b_t_U2th);
    fChain->SetBranchAddress("U2flag", &U2flag, &b_t_U2flag);
    fChain->SetBranchAddress("U2chisq", &U2chisq, &b_t_U2chisq);
    fChain->SetBranchAddress("U2res0", &U2res0, &b_t_U2res0);
    fChain->SetBranchAddress("U2res1", &U2res1, &b_t_U2res1);
    fChain->SetBranchAddress("U2effID", &U2effID, &b_t_U2effID);
    fChain->SetBranchAddress("U2hits", &U2hits, &b_t_U2hits);
    fChain->SetBranchAddress("U2effall", &U2effall, &b_t_U2effall);
    fChain->SetBranchAddress("U2effdt", &U2effdt, &b_t_U2effdt);
    fChain->SetBranchAddress("U2effgroup", &U2effgroup, &b_t_U2effgroup);
    fChain->SetBranchAddress("U2effgood", &U2effgood, &b_t_U2effgood);
    fChain->SetBranchAddress("ThSCAT", &ThSCAT, &b_t_ThSCAT);
    fChain->SetBranchAddress("ThFP", &ThFP, &b_t_ThFP);
    fChain->SetBranchAddress("PhiFP", &PhiFP, &b_t_PhiFP);
    fChain->SetBranchAddress("ThFPx", &ThFPx, &b_t_ThFPx);
    fChain->SetBranchAddress("Y1", &Y1, &b_t_Y1);
    fChain->SetBranchAddress("Y2", &Y2, &b_t_Y2);
    fChain->SetBranchAddress("pulser", &pulser, &b_t_pulser);
    fChain->SetBranchAddress("X1posC", &X1posC, &b_t_X1posC);
    fChain->SetBranchAddress("SiliconHits", &SiliconHits, &b_SiliconInfo_SiliconHits);
    fChain->SetBranchAddress("SiliconEnergy", &SiliconEnergy, &b_SiliconInfo_SiliconEnergy);
    fChain->SetBranchAddress("SiliconTheta", &SiliconTheta, &b_SiliconInfo_SiliconTheta);
    fChain->SetBranchAddress("SiliconPhi", &SiliconPhi, &b_SiliconInfo_SiliconPhi);
    fChain->SetBranchAddress("SiliconTime", &SiliconTime, &b_SiliconInfo_SiliconTime);
    fChain->SetBranchAddress("DetectorHit", &DetectorHit, &b_SiliconInfo_DetectorHit);
    fChain->SetBranchAddress("ADCChannelFront", &ADCChannelFront, &b_SiliconInfo_ADCChannelFront);
    fChain->SetBranchAddress("ADCChannelBack", &ADCChannelBack, &b_SiliconInfo_ADCChannelBack);
    fChain->SetBranchAddress("TDCChannelFront", &TDCChannelFront, &b_SiliconInfo_TDCChannelFront);
    fChain->SetBranchAddress("TDCChannelBack", &TDCChannelBack, &b_SiliconInfo_TDCChannelBack);
    fChain->SetBranchAddress("ADCValueFront", &ADCValueFront, &b_SiliconInfo_ADCValueFront);
    fChain->SetBranchAddress("ADCValueBack", &ADCValueBack, &b_SiliconInfo_ADCValueBack);
    fChain->SetBranchAddress("TDCValueFront", &TDCValueFront, &b_SiliconInfo_TDCValueFront);
    fChain->SetBranchAddress("TDCValueBack", &TDCValueBack, &b_SiliconInfo_TDCValueBack);
    fChain->SetBranchAddress("StripFront", &StripFront, &b_SiliconInfo_StripFront);
    fChain->SetBranchAddress("StripBack", &StripBack, &b_SiliconInfo_StripBack);
    fChain->SetBranchAddress("EnergyFront", &EnergyFront, &b_SiliconInfo_EnergyFront);
    fChain->SetBranchAddress("EnergyBack", &EnergyBack, &b_SiliconInfo_EnergyBack);
    fChain->SetBranchAddress("TDCChannelMultiplicity", &TDCChannelMultiplicity, &b_SiliconInfo_TDCChannelMultiplicity);
}

Bool_t Li2CO3::Notify()
{
    // The Notify() function is called when a new file is opened. This
    // can be either for a new TTree in a TChain or when when a new TTree
    // is started when using PROOF. It is normally not necessary to make changes
    // to the generated code, but the routine can be extended by the
    // user if needed. The return value is currently not used.

    return kTRUE;
}

#endif // #ifdef Li2CO3_cxx
