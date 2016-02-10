TCut basecut = "!X1flag && !U1flag && !X2flag && !U2flag && X1chisq < 0.2";
TCut pidcut = "CUTpid";

void Spectra()
{
    extern bool runsEmbedPID;
    if (runsEmbedPID)
    {
        pidcut = "PIDgood";
    }
    basecut = basecut && pidcut;
}

void ExSi(TCut cut = "", TString name = "ExSi")
{
    DATA->Draw("SiliconEnergy:Ex>>h_"+name+"(800,4,16,1000,0,10000)", basecut && cut, "col");
}

void SiliconTime(TCut cut = "")
{
    //DATA->Draw("SiliconTime-toftdc7>>h_SiliconTime(1000,-9000,9000)", basecut && cut);
    DATA->Draw("SiliconTime-toftdc7>>h_SiliconTime(100,-900,900)", basecut && cut);
}

void PID()
{
    DATA->Draw("pad1:tof>>h_PID(1000,1000,9000,1000,0,3000)", "", "colz");
}

void Ex(TCut cut = "")
{
    DATA->Draw("Ex>>h_Ex(800,4,16)", basecut && cut);
}

void X1pos(TCut cut = "")
{
    DATA->Draw("X1pos>>h_X1(1600,0,800)", basecut && cut);
}

void SiHitPattern(TCut cut = "")
{
    DATA->Draw("ADCChannelBack:ADCChannelFront>>h_SiHit(224,0,224,224,0,224)", basecut && cut, "col");
}

void SiFrontBack_PD(TCut cut = "")
{
    TCanvas* cSiFB = new TCanvas("cSiFB");
    cSiFB->Divide(2, 2);
    cSiFB->cd(1);
    DATA->Draw("EnergyBack:EnergyFront>>h_SiFB5(1000,0,10000,1000,0,10000)", basecut && cut && "DetectorHit == 5", "col");
    cSiFB->cd(2);
    DATA->Draw("EnergyBack:EnergyFront>>h_SiFB6(1000,0,10000,1000,0,10000)", basecut && cut && "DetectorHit == 6", "col");
    cSiFB->cd(3);
    DATA->Draw("EnergyBack:EnergyFront>>h_SiFB7(1000,0,10000,1000,0,10000)", basecut && cut && "DetectorHit == 7", "col");
    cSiFB->cd(4);
    DATA->Draw("EnergyBack:EnergyFront>>h_SiFB8(1000,0,10000,1000,0,10000)", basecut && cut && "DetectorHit == 8", "col");
}

void CheckLUT()
{
    TCanvas* cLUT = new TCanvas("cLUT");
    cLUT->Divide(2, 2);
    cLUT->cd(1);
    DATA->Draw("X1res1:X1res0>>h_lutX1(100,0,1,100,-5,5)", pidcut, "colz");
    cLUT->cd(2);
    DATA->Draw("U1res1:U1res0>>h_lutU1(100,0,1,100,-5,5)", pidcut, "colz");
    cLUT->cd(3);
    DATA->Draw("X2res1:X2res0>>h_lutX2(100,0,1,100,-5,5)", pidcut, "colz");
    cLUT->cd(4);
    DATA->Draw("U2res1:U2res0>>h_lutU2(100,0,1,100,-5,5)", pidcut, "colz");
}
