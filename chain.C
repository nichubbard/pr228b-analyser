{
    TChain chain("DATA");
    chain.Add("run00124.root");
    chain.Add("run00125.root");
    chain.Add("run00126.root");
    chain.Add("run00128.root");
    chain.Add("run00129.root");
    chain.Add("run00130.root");
    chain.Add("run00132.root");
    //chain.Add("run00150.root");

    TProof* proof = TProof::Open("lite://");
    chain.SetProof();
    //proof->Load("load_cuts.C,CUTalphas.root,CUTalphastate4.root,CUTalphastate5.root,CUTepair.root,CUTpid.root,CUTli.root");
    //proof->Exec("load_cuts()");
    //load_cuts();

    TTree* DATA = &chain;
}
