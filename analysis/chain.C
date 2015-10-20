{
    const int runs[] = {
        //124, 126, 128, 129, 138
        1071, 1084, 1098
    };
    const size_t n_runs = sizeof(runs) / sizeof(runs[0]);
    TChain chain("DATA");
    for (size_t i = 0; i < n_runs; ++i)
    {
        char buf[32];
        sprintf(buf, "run%05d.root", runs[i]);
        chain.Add(buf);
    }

    TProof* proof = TProof::Open("lite://");
    chain.SetProof();
    //proof->Load("load_cuts.C,CUTalphas.root,CUTalphastate4.root,CUTalphastate5.root,CUTepair.root,CUTpid.root,CUTli.root");
    //proof->Exec("load_cuts()");
    //load_cuts();

    TTree* DATA = &chain;
}
