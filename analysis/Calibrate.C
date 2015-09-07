#define Calibrate_cxx
// The class definition in Calibrate.h has been generated automatically
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
// Root > T->Process("Calibrate.C")
// Root > T->Process("Calibrate.C","some options")
// Root > T->Process("Calibrate.C+")
//
// This file is for automatically calibrating a set of DSSSDs from the
// RawInfo/SiliconInfo data from k600analyser. Designed for PR228 but
// should be flexible enough for other data. Most changes made should
// be possible with the variables just below the includes.
//
// The calibration data will be saved both as a CSV file for general
// analysis and as a preformed C file (SiliconParameters.c), of two
// arrays which can be directly inserted into Parameters.c for calibration
//

#include "Calibrate.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <TCanvas.h>
#include <TError.h>
#include <TF1.h>
#include <TGraphErrors.h>
#include <TH2.h>
#include <TMath.h>
#include <TSpectrum.h>
#include <TSpline.h>
#include <TStyle.h>

//#include "srim.h"

// First channel to start fitting to
static const int channel_start = 8;
// Number of ADC channels to fit to
static const int channels = 16;
// Alpha energies for each detected peak (in ascending order)
//static const double alpha_energies[] = { 4784.34, 5489.48, 6002.35, 7686.82 };
//Number of alpha peaks (size of alpha_energies array)
//static const int alpha_peaks = 4;
//static const double alpha_energies[] = { 5340.36, 5685.37, 6050.78, 6778.3, 8784.86 };
//static const int alpha_peaks = 5;
static const double alpha_energies[] = { 662, 1170, 1330 };
static const int alpha_peaks = 3;

// Perform calibration on SiliconInfo entries (filtered as particles) rather
// than RawInfo entries (raw ADC values)
static const bool use_silicon = false;

// Also fit a deadlayer parameter (experimental)
static const bool use_deadlayer = false;

// Attempt to remove background and noise from the histograms before doing
// the peak search
static const bool remove_background = false;

// If true then graphs of the calibration routine will be saved in the folder
// "calibration". Each ADc channel will have 3 graphs:
// - ADCChannelN - Raw ADC histogram for channel N
// - GaussChannelN - Smoothed/BG-Removed histogram with Gaussian fitted
// - LinearChannelN - Linear fit to the peak vs alpha energy graph
static const bool save_graphs = true;

// Events with an ADC value below this threshold will not be included in the
// peak fitting histograms
static const int threshold = 500;

// The number of bins, start and end value for the histograms
static const int h_bins = 4096 / 4;
static const int h_start = 0;
static const int h_end = 4096;

//
// The following parameters control the peak detection, including
// background removal and smoothing. You will likely need trial and error
// here to identify the optimal values for the data
//

// The resolution of neighbouring peaks, higher values permit smaller
// distance between peaks
static const float spectrum_resolution = 1.;
// The number of iterations for the background scan. More iterations
// smooths and lowers the result
static const int spectrum_bg_iterations = 60;
// Number of times to smooth the BG-removed histogram before peak fitting
static const int spectrum_pre_smooth = 2;
// The sigma of the searched peaks
static const double spectrum_sigma = 2;
// The threshold of detected peaks, peaks less than threshold*height of
// the largest peak are ignored
static const double spectrum_threshold = 0.10;

// Calibration data for each ADC Channel
struct CalibrateData
{
    // Histogram of AdcValues for this channel
    TH1F* Histogram;
    // Centroid ADC value for each alpha peak
    float AdcValue[alpha_peaks];
    // Sigma (error) ADC value for each alpha peak
    float AdcError[alpha_peaks];
    // Energy for each alpha peak
    float Energy[alpha_peaks];
    // Error in energy for each alpha peak (always 0)
    float EnergyError[alpha_peaks];

    // Debugging Data
    int Peaks;

    // Calibration Gain and Offset
    float Gain;
    float Offset;
    float DeadLayer;
};

static TSpline3* srim_spline;

double fpeaks(double* x, double* par)
{
    double result = 0.;
    int peaks = par[0];
    for (int i = 0; i < peaks; ++i)
    {
        double norm  = par[3 * i + 1];
        double mean  = par[3 * i + 2];
        double sigma = par[3 * i + 3];
        result += norm * TMath::Gaus(x[0], mean, sigma);
    }
    return result;
}

class DeadLayerFit
{
    public:
        DeadLayerFit(double _gain, double _offset)
            : gain(_gain), offset(_offset)
        {
        }

        double operator()(double* x, double* par)
        {
            double channel = x[0];
            double result = par[0] * channel + par[1]
                + srim_spline->Eval(gain * channel + offset) * par[2];
            return result;
        }
    private:
        double gain;
        double offset;
};

CalibrateData data[channels];

void Calibrate::Begin(TTree * /*tree*/)
{
    // The Begin() function is called at the start of the query.
    // When running with PROOF Begin() is only called on the client.
    // The tree argument is deprecated (on PROOF 0 is passed).

    TString option = GetOption();
    std::cout << "Generating histograms" << std::endl;
    for (int i = 0; i < channels; ++i)
    {
        std::stringstream name;
        name << "h" << i;
        //gFile->Delete(name.str().c_str());
        data[i].Histogram = new TH1F(name.str().c_str(), "",
                h_bins, h_start, h_end);
    }
    //srim_spline = new TSpline3("", SRIM_E, SRIM_DEDX, SRIM_N);
    gErrorIgnoreLevel = kError;
}

void Calibrate::SlaveBegin(TTree * /*tree*/)
{
    // The SlaveBegin() function is called after the Begin() function.
    // When running with PROOF SlaveBegin() is called on each slave server.
    // The tree argument is deprecated (on PROOF 0 is passed).

    TString option = GetOption();

}

Bool_t Calibrate::Process(Long64_t entry)
{
    // The Process() function is called for each entry in the tree (or possibly
    // keyed object in the case of PROOF) to be processed. The entry argument
    // specifies which entry in the currently loaded tree is to be processed.
    // It can be passed to either Calibrate::GetEntry() or TBranch::GetEntry()
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


    if (use_silicon)
    {
        //b_SiliconInfo_ADCChannelFront->GetEntry(entry);
        //b_SiliconInfo_ADCChannelBack->GetEntry(entry);
        //b_SiliconInfo_ADCValueFront->GetEntry(entry);
        //b_SiliconInfo_ADCValueBack->GetEntry(entry);
        //for (size_t i = 0; i < ADCChannelBack.size(); ++i)
        //{
            //int front = ADCChannelFront[i];
            //int back = ADCChannelBack[i];
            //int front_val = ADCValueFront[i];
            //int back_val = ADCValueBack[i];
            //data[front].Histogram->Fill(front_val);
            //data[back].Histogram->Fill(back_val);
        //}
    }
    else
    {
        b_RawInfo_ADCValues->GetEntry(entry);
        for (size_t i = 0; i < ADCValues.size(); ++i)
        {
            int val = ADCValues[i];
            if (i >= channels)
                continue;
            if (val > threshold && val < 3900)
            {
                data[i].Histogram->Fill(val);
            }
        }
    }
    return kTRUE;
}

void Calibrate::SlaveTerminate()
{
    // The SlaveTerminate() function is called after all entries or objects
    // have been processed. When running with PROOF SlaveTerminate() is called
    // on each slave server.

}

void Calibrate::Terminate()
{
    // The Terminate() function is the last function to be called during
    // a query. It always runs on the client, it can be used to present
    // the results graphically or save the results to file.

    using std::cout;
    using std::endl;
    using std::flush;

    TCanvas* c1 = new TCanvas("c1");
    for (int i = channel_start; i < channels; ++i)
    {
        cout << "Analysing channel #" << i << "\r" << flush;

        data[i].Histogram->SetTitle("Channel #" + TString::LLtoa(i, 10));
        data[i].Histogram->GetXaxis()->SetTitle("Channel Number");
        data[i].Histogram->GetYaxis()->SetTitle("Counts");

        if (save_graphs)
        {
            data[i].Histogram->Draw("");
            c1->Print("../analysis/calibration/ADCChannel" + TString::LLtoa(i, 10) + ".png",
                    "png");
        }

        TSpectrum* s = new TSpectrum(2 * alpha_peaks, spectrum_resolution);
        double* bg = 0;
        TH1F* back = 0;
        if (remove_background)
        {
            bg = new double[h_bins];
            for (int j = 0; j < h_bins; ++j)
                bg[j] = data[i].Histogram->GetBinContent(j + 1);
            s->Background(bg, h_bins, spectrum_bg_iterations,
                    TSpectrum::kBackDecreasingWindow,
                    TSpectrum::kBackOrder4, kTRUE,
                    TSpectrum::kBackSmoothing15, kFALSE);
            back = new TH1F("back", "", h_bins, h_start, h_end);
            for (int j = 0; j < h_bins; ++j)
                back->SetBinContent(j + 1, bg[j]);

            data[i].Histogram->Add(back, -1);
            data[i].Histogram->Smooth(spectrum_pre_smooth);
        }

        int peaks = s->Search(data[i].Histogram, spectrum_sigma, "noMarkov",
                spectrum_threshold);

        double* peakx = s->GetPositionX();
        float* sorted = new float[peaks];
        // Sort the detected peaks into ascending order so they can
        // be matched up with the alpha energy array later
        std::copy(&peakx[0], &peakx[peaks], &sorted[0]);
        std::sort(&sorted[0], &sorted[peaks]);
        // TODO: Make this configurable (sub_peaks array?)
        //if (peaks == 7)
        //{
             //Cool ugly hack to remove a side-peak (third peak)
            //float* new_sorted = new float[6];
            //std::copy(&sorted[1], &sorted[peaks], &new_sorted[0]);
            //peaks = 6;
            //delete[] sorted;
            //sorted = new_sorted;
        //}
        //if (peaks == 6)
        //{
             //Cool ugly hack to remove a side-peak (third peak)
            //float* new_sorted = new float[5];
            //std::copy(&sorted[0], &sorted[2], &new_sorted[0]);
            //std::copy(&sorted[3], &sorted[peaks], &new_sorted[2]);
            //peaks = 5;
            //delete[] sorted;
            //sorted = new_sorted;
        //}
        if (peaks == 6)
        {
            float* new_sorted = new float[3];
            std::copy(&sorted[0], &sorted[1], &new_sorted[0]);
            std::copy(&sorted[4], &sorted[6], &new_sorted[1]);
            peaks = 3;
            delete[] sorted;
            sorted = new_sorted;
        }

        double* par = new double[1 + peaks * 3];
        par[0] = peaks;
        for (int j = 0; j < peaks; ++j)
        {
            float xp = sorted[j];
            int bin = data[i].Histogram->GetXaxis()->FindBin(xp);
            float yp = data[i].Histogram->GetBinContent(bin);
            // Initial guesses for fit
            par[j * 3 + 1] = yp; // norm
            par[j * 3 + 2] = xp; // mean
            par[j * 3 + 3] = 3;  // sigma
        }
        data[i].Peaks = peaks;

        TF1* fit = new TF1("fit", fpeaks, h_start, h_end, 1 + 3 * peaks);
        fit->SetParameters(par);
        fit->FixParameter(0, peaks);
        fit->SetNpx(1000);
        data[i].Histogram->Fit("fit", "Q");

        if (save_graphs)
        {
            c1->Print("../analysis/calibration/GaussChannel" + TString::LLtoa(i, 10) + ".png",
                "png");
        }

        for (int j = 0; j < alpha_peaks; ++j)
        {
            data[i].AdcValue[j] = fit->GetParameter(3 * j + 2);
            data[i].AdcError[j] = fit->GetParameter(3 * j + 3);
            data[i].Energy[j] = alpha_energies[j];
            data[i].EnergyError[j] = 1.;
        }

        delete fit;
        delete[] bg;
        delete back;
        delete[] sorted;
        delete[] par;
        delete s;
        delete data[i].Histogram;
        data[i].Histogram = 0;
    }

    cout << "Peak analysis complete" << endl;

    for (int i = channel_start; i < channels; ++i)
    {
        cout << "Fitting channel #" << i << "\r" << flush;
        TGraphErrors* gr = new TGraphErrors(alpha_peaks, data[i].AdcValue,
                data[i].Energy, data[i].AdcError, data[i].EnergyError);
        TF1* pol = new TF1("pol", "pol1", h_start, h_end);
        gr->Fit("pol", "FQ");
        TF1* fit;
        if (use_deadlayer)
        {
            DeadLayerFit deadlayer(pol->GetParameter(1), pol->GetParameter(0));
            fit = new TF1("fit", deadlayer, h_start, h_end, 3);
            fit->SetParameters(pol->GetParameter(1), pol->GetParameter(0), 0.2);
            fit->SetParLimits(2, 0, 5);
            fit->SetParNames("Gain", "Offset", "Dead Layer");
        }
        else
        {
            fit = pol;
            fit->SetName("fit");
            fit->SetParNames("Offset", "Gain");
        }
        gr->SetTitle("Channel #" + TString::LLtoa(i, 10));
        gr->GetXaxis()->SetTitle("Channel Number");
        gr->GetYaxis()->SetTitle("Alpha Energy (keV)");
        if (save_graphs)
        {
            gr->Draw("AP");
        }
        gr->Fit("fit", "FBQ");
        if (save_graphs)
        {
            c1->Print("../analysis/calibration/LinearChannel" + TString::LLtoa(i, 10) + ".png",
                "png");
        }
        if (use_deadlayer)
        {
            data[i].Gain = fit->GetParameter(0);
            data[i].Offset = fit->GetParameter(1);
            data[i].DeadLayer = fit->GetParameter(2);
            delete pol;
        }
        else
        {
            data[i].Gain = fit->GetParameter(1);
            data[i].Offset = fit->GetParameter(0);
            data[i].DeadLayer = 0;
        }
        delete fit;
        delete gr;
    }
    cout << "Fitting complete" << endl;

    cout << "Creating C file" << endl;
    std::ofstream output;
    output.open("../analysis/SiliconParameters.c");

    output << "double SiliconOffset[" << channels << "] = { ";
    for (int i = 0; i < channels; ++i)
    {
        output << data[i].Offset;
        if (i < channels - 1)
            output << ",";
    }
    output << "};" << endl << endl;

    output << "double SiliconGain[" << channels << "] = { ";
    for (int i = 0; i < channels; ++i)
    {
        output << data[i].Gain;
        if (i < channels - 1)
            output << ",";
    }
    output << "};" << endl << endl;

    output.close();
    cout << "Calibration completed and saved to SiliconParameters.c" << endl;

    cout << "The following channels did not calibrate properly" << endl;
    for (int i = channel_start; i < channels; ++i)
    {
        if (data[i].Peaks != alpha_peaks)
        {
            cout << "#" << i << "(" << data[i].Peaks << ")" << endl;
            data[i].Gain = 0.;
            data[i].Offset = 0.;
        }
    }

    output.open("../analysis/Silicon.csv");
    output << "Channel,Gain,Offset,DeadLayer" << endl;
    for (int i = 0; i < channels; ++i)
    {
        output << i << "," << data[i].Gain << "," << data[i].Offset << "," << data[i].DeadLayer << endl;
    }
    output.close();

    output.open("../analysis/CalibParsPR228.dat");
    for (int i = 0; i < channels; ++i)
    {
        output << i << " " << data[i].Gain << " " << data[i].Offset << endl;
    }
    output << "eof";
    output.close();

    delete c1;
    if (use_deadlayer)
    {
        TCanvas* dl = new TCanvas("deadlayers");
        dl->Divide(2, 2);
        dl->Show();
        for (int i = 0; i < 4; ++i)
        {
            dl->cd(i + 1);
            double channel[32];
            double deadlayer[32];
            for (int j = 0; j < 16; ++j)
            {
                channel[j] = j;
                channel[j + 16] = j + 16;
                deadlayer[j] = data[i * 16 + j].DeadLayer;
                deadlayer[j + 16] = data[64 + i * 16 +j].DeadLayer;
            }
            TGraph* gr = new TGraph(32, channel, deadlayer);
            gr->SetTitle("Detector #" + TString::LLtoa(i, 10));
            gr->GetXaxis()->SetTitle("Strip Number");
            gr->GetXaxis()->SetRangeUser(0, 32);
            gr->GetYaxis()->SetTitle("Dead Layer (um)");
            gr->Draw("A*");
        }
        dl->Update();
    }
}

