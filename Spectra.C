void Spectra()
{
}

void ExSi(TCut cut = "", TString name = "ExSi")
{
    DATA->Draw("SiliconEnergy:Ex>>h_"+name+"(800,4,16,1000,0,10000)", "CUTpid && SiliconTime-tof >= -1600 && SiliconTime-tof <= -1200" && cut, "col");
}

void SiliconTime()
{
    DATA->Draw("SiliconTime-tof>>h_SiliconTime(1000,-3000,3000)", "CUTpid");
}

void PID()
{
    DATA->Draw("pad1:tof>>h_PID(1000,3000,5000,1000,0,3000)", "", "colz");
}

void Ex()
{
    DATA->Draw("Ex>>h_Ex(800,4,16)", "CUTpid");
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
