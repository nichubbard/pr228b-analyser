#include <TF1.h>
#include <TH1F.h>
#include <iostream>

using namespace std;

void GausFit()
{
}

template <typename TH1X>
void GausFit(TH1X* h, float x1, float x2, float bg1 = 0, float bg2 = 0)
{
    if (bg1 == 0)
        bg1 = x1;
    if (bg2 == 0)
        bg2 = x2;

    TF1* fit = new TF1("fit", "gaus");
    h->Fit("fit", "QN", "", x1, x2);
    double a = fit->GetParameter(0);
    double mu = fit->GetParameter(1);
    double sigma = fit->GetParameter(2);
    delete fit;
    fit = new TF1("fit", "gaus(0)+pol1(3)");
    fit->SetParameter(0, a);
    fit->SetParameter(1, mu);
    fit->SetParameter(2, sigma);
    h->Fit("fit", "QM", "", bg1, bg2);

    a = fit->GetParameter(0);
    mu = fit->GetParameter(1);
    sigma = fit->GetParameter(2);
    double c = fit->GetParameter(3);
    double m = fit->GetParameter(4);

    TF1* bgfit = new TF1("bgfit", "pol1");
    bgfit->FixParameter(0, c);
    bgfit->FixParameter(1, m);
    bgfit->SetLineColor(kGreen);
    h->Fit("bgfit", "QF+", "", bg1, bg2);

    int binLeft = h->GetXaxis()->FindBin(x1);
    int binRight = h->GetXaxis()->FindBin(x2);
    double bg = bgfit->Integral(x1, x2) / h->GetBinWidth(1);
    double peak = h->Integral(binLeft, binRight);
    cout << "Background: " << m << "x + " << c << endl;
    cout << " Integral: " << bg << endl;
    cout << "Peak: a=" << a << ", mu=" << mu << ", s=" << sigma << endl;
    cout << " Integral: " << (peak - bg) << endl;
}

