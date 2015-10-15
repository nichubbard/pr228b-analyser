void DblInt()
{
   Ex();
   TH1F* h = (TH1F*)gDirectory->FindObject("h_Ex");
   //TF1* fit = new TF1("fd", "gausn(0)+gausn(3)+pol0(6)");
   TF1* fit = new TF1("fd", "[0]*exp(-0.5*((x-[1])/[2])**2)/(sqrt(2*pi)*[2])+[3]*exp(-0.5*((x-[4])/[2])**2)/(sqrt(2*pi)*[2])+pol0(5)");
   fit->GetFormula()->Compile();
   fit->SetParameter(0, 5);
   fit->SetParameter(1, 6.049);
   fit->SetParameter(2, 0.02);
   fit->SetParameter(3, 15);
   fit->SetParameter(4, 6.129);
   fit->SetParameter(5, 400);

   h->Fit(fit, "FM", "", 5.9, 6.3);

   TF1* fit2 = new TF1("fit_first_gaussian", "gausn(0)+pol0(3)");
   fit2->FixParameter(0, fit->GetParameter(0));
   fit2->FixParameter(1, fit->GetParameter(1));
   fit2->FixParameter(2, fit->GetParameter(2));
   fit2->FixParameter(3, fit->GetParameter(5));
   fit2->SetLineColor(kBlack);
   h->Fit(fit2, "FM+", "", 5.9, 6.3);

   TF1* fit3 = new TF1("fit_second_gaussian", "gausn(0)+pol0(3)");
   fit3->FixParameter(0, fit->GetParameter(3));
   fit3->FixParameter(1, fit->GetParameter(4));
   fit3->FixParameter(2, fit->GetParameter(2));
   fit3->FixParameter(3, fit->GetParameter(5));
   fit3->SetLineColor(kGreen);
   h->Fit(fit3, "FM+", "", 5.9, 6.3);

   double zeroplus = fit->GetParameter(0);
   double threeminus = fit->GetParameter(3);
   cout << "0+ State: " << zeroplus << endl;
   cout << "3- State: " << threeminus << endl;
   cout << "Ratio   : " << zeroplus / (zeroplus + threeminus) << endl;
}
