#include "config.cpp++"

TChain chain("DATA");
TProof* proof;
TTree* DATA;
bool runsEmbedPID;

void runs()
{
   gErrorIgnoreLevel = kWarning;
   runsEmbedPID = false;
   AnalysisConfig& config = AnalysisConfig::Instance();

   for (auto i : config.Runs())
   {
      char buf[32];
      sprintf(buf, "run%05d.root", i);
      chain.Add(buf);
   }

   if (config.UsePROOF())
   {
      int proofWorkers = config.PROOFWorkers();
      if (proofWorkers > 0)
      {
         char format[32];
         sprintf(format, "workers=%d", proofWorkers);
         proof = TProof::Open(format);
      }
      else
      {
         proof = TProof::Open("");
      }
      chain.SetProof();
      proof->Exec("gErrorIgnoreLevel = kWarning;");
      proof->Load("config.cpp+");
   }
   if (config.UsePID())
   {
      if (config.PIDfile() == "TREE")
      {
         runsEmbedPID = true;
      }
      else
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
   }

   DATA = &chain;
}
