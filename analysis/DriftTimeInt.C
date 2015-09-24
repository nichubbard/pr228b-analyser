//#include <iostream>
// use e.g.                root[] h1=hU1_DriftTimeGood
{
  ofstream out;
  out.open("lutfile");

  float lut;
  Int_t nbins = h1->GetNbinsX();

  // First set bincontent to 0 for those bins that should not form
  // part of the integration
  // Dec09: X1:6500 8200   X2:6500 8200   U1:6500 8200  U2:6500 8000
  // this is off course made redundent if you take the spectra made clean
  // in the analysis
  //
  // To do:
  // TH1F *h1 = new TH1F("h1","drifttime",14999,0.,14999.);
  // Then put the appropriete drifttime into h1:
  // h1=hX1_DriftTimeGood
  // Now execute this script.

/*
  for(Int_t j=0;j<6400;j++) {
    h1->SetBinContent(j,0);
  }
  for(Int_t j=8000;j<nbins;j++) {
    h1->SetBinContent(j,0);
  }
*/

  // Now do the integration
  for(Int_t i=0;i<nbins;i++) {
    lut = 1.0-(h1->Integral(0,i)/h1->Integral(0,nbins));
    //printf("%g\n",1.0-(h1->Integral(0,i)/h1->Integral(0,nbins)));
    out << lut  << endl;
  }

  out.close();
}
