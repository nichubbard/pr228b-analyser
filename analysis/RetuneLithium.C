// Magnetic rigidities of the various states in Li2CO3 that we fit
// The X1 positions to.
// The sort code will convert rigidities into excitation energies based
// on input target and beam data. This kindly allows one calibration
// to apply to both Li2CO3 and Carbon runs.
double rigidities[] = {
    3.013127, // 16O 12    MeV 0+
    3.020718, // 16O 11.5  MeV 2+
    3.047242, // 12C 9.641 MeV 3-
    3.075348, // 12C 7.65  MeV 0+
    3.08303,  // 16O 7.117 MeV 1-
    3.085813  // 16O 6.129 MeV 3-
};

void RetuneLithium()
{
    chain.SetProof(0);
    // Get X1position graph from the Spectra.C macro
    X1pos((TCut)"Ex > 4 && Ex < 13");
    TH1F* h_X1 = (TH1F*)gDirectory->FindObject("h_X1");

    // Find peaks, and sort the resulting array in ascending X1pos
    TSpectrum* ts = new TSpectrum();
    int peaks = ts->Search(h_X1);
    double* x = ts->GetPositionX();
    std::sort(&x[0], &x[peaks]);

    TCanvas* c1 = new TCanvas("c_X1");
    h_X1->Draw();

    // Filter out "peaks" very close to the edge of the cut
    // First look for first bin with any counts in, and get the X1pos
    double start_x = 0;
    for (int i = 0; i < h_X1->GetNbinsX(); ++i)
    {
        if (h_X1->GetBinContent(i) > 0)
        {
            start_x = h_X1->GetBinCenter(i);
            break;
        }
    }
    // Add a small amount to the edge to ignore the peaks close by
    start_x += 10;
    // And look for the first X peak found after the start condition
    int start_idx = 0;
    for (int i = 0; i < peaks; i++)
    {
        if (x[i] > start_x)
        {
            start_idx = i;
            break;
        }
    }

    // Need 7 peaks remaining after the start cut
    // The last peak is the 0+/3- peak that isnt fitted because its
    // an unresolved doublet.
    // If more found, later conditions will assume some weaker peaks
    // have been found.
    if (peaks - start_idx == 7)
    {
        TCanvas* c2 = new TCanvas("c_Graph");
        TGraph* graph = new TGraph(6, &x[start_idx], rigidities);
        graph->Fit("pol2");
        graph->Draw("A*");
    }
    else
    {
        std::cout << "You need to modify the peaks" << std::endl;
    }
}
