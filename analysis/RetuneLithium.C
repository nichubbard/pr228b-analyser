double energies[] = {
    -1,
    641.8,
    631.4,
    603.7,
    502.9,
    494.2,
    409.4
};

double rigidities[] = {
    3.013127,
    3.020718,
    3.047242,
    3.075348,
    3.08303,
    3.085813
};

void RetuneLithium()
{
    X1pos((TCut)"Ex > 4 && Ex < 13");
    TSpectrum* ts = new TSpectrum();
    TH1F* h_X1 = (TH1F*)gDirectory->FindObject("h_X1");
    int peaks = ts->Search(h_X1);
    TCanvas* c1 = new TCanvas("c_X1");
    h_X1->Draw();
    TCanvas* c2 = new TCanvas("c_Graph");
    if (peaks >= 8)
    {
        double* x = ts->GetPositionX();
        std::sort(&x[0], &x[peaks]);
        TGraph* graph = new TGraph(6, &x[1], rigidities);
        graph->Fit("pol2");
        graph->Draw("A*");
    }
    else
    {
        std::cout << "You need to modify the peaks" << std::endl;
    }
}
