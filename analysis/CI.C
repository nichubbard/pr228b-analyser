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
        TH1F* Scaler_CIU = (TH1F*)file->Get("Scaler_CII");
        TH1F* Scaler_ClockU = (TH1F*)file->Get("Scaler_ClockI");
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
    double C = sumCI * R * 1E-3;
    cout << "Total C    = " << C << " nC" << endl;
    double N = sumCI * R * scale / (Z * e);
    cout << "Total N    = " << N << endl;
    double I = C / sumClock * 10;
    cout << "Average I  = " << I << " nA" << endl;
}

