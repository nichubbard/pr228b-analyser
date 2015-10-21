#include "config.cpp+"

TChain chain("DATA");
TProof* proof;
TTree* DATA;

void runs()
{
   AnalysisConfig config;

   for (auto i : config.Runs())
   {
      char buf[32];
      sprintf(buf, "run%05d.root", i);
      chain.Add(buf);
   }

   if (config.UsePROOF())
   {
      proof = TProof::Open("lite://");
      chain.SetProof();
   }
   //proof->Load("load_cuts.C,CUTalphas.root,CUTalphastate4.root,CUTalphastate5.root,CUTepair.root,CUTpid.root,CUTli.root");
   //proof->Exec("load_cuts()");
   //load_cuts();

   DATA = &chain;
}
