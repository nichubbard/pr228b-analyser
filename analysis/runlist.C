#include "config.cpp++"

void runlist()
{
   gErrorIgnoreLevel = kWarning;
   AnalysisConfig& config = AnalysisConfig::Instance();

   for (auto i : config.Runs())
   {
       cout << i << " ";
   }
   cout << endl;
}
