#include "config.cpp+"

TChain chain("DATA");
TProof* proof;
TTree* DATA;

void runs()
{
   AnalysisConfig& config = AnalysisConfig::Instance();

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
   if (config.UsePID())
   {
      std::string pid = config.PIDfile() + ".C";
      if (config.UsePROOF())
      {
         proof->Load(pid.c_str());
         proof->Exec("delete c1");
      }
      else
      {
         gROOT->LoadMacro(pid.c_str());
      }
      delete gDirectory->FindObject("c1");
   }

   DATA = &chain;
}
