#include "config.cpp++"

void CI()
{
    int sumCI = 0;
    int sumClock = 0;
    AnalysisConfig& config = AnalysisConfig::Instance();

    for (auto i : config.Runs())
    {
        char buf[32];
        sprintf(buf, "run%05d.root", i);
        TFile* file = TFile::Open(buf);
        TH1F* Scaler_CIU = (TH1F*)file->Get("Scaler_CIU");
        TH1F* Scaler_ClockU = (TH1F*)file->Get("Scaler_ClockU");
        sumCI += Scaler_CIU->GetEntries();
        sumClock += Scaler_ClockU->GetEntries();
        delete file;
    }

    cout << "Total CI   = " << sumCI << endl;
    cout << "Total time = " << sumClock << endl;
    double R = 20.;
    double scale = 1E-12;
    double e = 1.6E-19;
    double Z = 2;
    double N = sumCI * R * scale / (Z * e);
    cout << "Total N    = " << N << endl;
}

