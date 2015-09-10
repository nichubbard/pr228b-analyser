void Spectra()
{
}

void ExSi(TCut cut = "", TString name = "ExSi")
{
    DATA->Draw("SiliconEnergy:Ex>>h_"+name+"(800,4,16,1000,0,10000)", "CUTpid" && cut, "col");
}

void SiliconTime(TCut cut = "")
{
    DATA->Draw("SiliconTime-tof>>h_SiliconTime(1000,-9000,9000)", "CUTpid" && cut);
}

void PID()
{
    DATA->Draw("pad1:tof>>h_PID(1000,1000,9000,1000,0,3000)", "", "colz");
}

void Ex()
{
    DATA->Draw("Ex>>h_Ex(800,4,16)", "CUTpid && !X1flag && !U1flag && !X2flag && !U2flag && X1chisq < 0.2");
}

void X1pos(TCut cut = "")
{
    DATA->Draw("X1pos>>h_X1(1600,0,800)", "CUTpid" && cut);
}

void CheckLUT()
{
    TCanvas* cLUT = new TCanvas("cLUT");
    cLUT->Divide(2, 2);
    cLUT->cd(1);
    DATA->Draw("X1res1:X1res0>>h_lutX1(100,0,1,100,-5,5)", "CUTpid", "colz");
    cLUT->cd(2);
    DATA->Draw("U1res1:U1res0>>h_lutU1(100,0,1,100,-5,5)", "CUTpid", "colz");
    cLUT->cd(3);
    DATA->Draw("X2res1:X2res0>>h_lutX2(100,0,1,100,-5,5)", "CUTpid", "colz");
    cLUT->cd(4);
    DATA->Draw("U2res1:U2res0>>h_lutU2(100,0,1,100,-5,5)", "CUTpid", "colz");
}
