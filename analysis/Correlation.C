void Correlation()
{
    TCanvas* c1 = new TCanvas("c1");
    c1->Divide(2, 2);
    int entries= DATA->GetEntries();

    TString e1 = TString::LLtoa(1 * entries / 4, 10);
    TString e2 = TString::LLtoa(2 * entries / 4, 10);
    TString e3 = TString::LLtoa(3 * entries / 4, 10);
    TString e4 = TString::LLtoa(4 * entries / 4, 10);

    c1->cd(1);
    ExSi((TCut)("Entry$ >= 0 && Entry$ < " + e1), "ExSi1");
    c1->cd(2);
    ExSi((TCut)("Entry$ >= " + e1 + " && Entry$ < " + e2), "ExSi2");
    c1->cd(3);
    ExSi((TCut)("Entry$ >= " + e2 + " && Entry$ < " + e3), "ExSi3");
    c1->cd(4);
    ExSi((TCut)("Entry$ >= " + e3 + " && Entry$ < " + e4), "ExSi4");
}
