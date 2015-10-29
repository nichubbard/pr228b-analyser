#include "config.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

AnalysisConfig* AnalysisConfig::instance = nullptr;

static bool get_bool(std::string value)
{
   std::transform(value.begin(), value.end(), value.begin(), ::tolower);
   if (value == "yes" || value == "true")
      return true;
   return false;
}

AnalysisConfig::AnalysisConfig()
{
   std::ifstream file("analysis.cfg");
   std::string line;

   while (file.good())
   {
      getline(file, line);
      if (line.substr(0, 1) == "#") continue;
      if (line.empty()) continue;
      auto first_space = line.find_first_of(" ");
      std::string name = line.substr(0, first_space);
      std::string value = line.substr(first_space + 1);

      if (name == "Runs")
      {
         std::size_t pos = 0;
         runs.clear();
         while(pos != std::string::npos)
         {
            std::size_t newpos = value.find(',', pos);
            std::string run = value.substr(pos, newpos - pos);
            int r = std::stoi(run);
            runs.push_back(r);
            pos = newpos;
            if (pos != std::string::npos) pos += 1;
         }
      }
      else if (name == "UsePROOF")
      {
         usePROOF = get_bool(value);
      }
      else if (name == "UsePID")
      {
         usePID = get_bool(value);
      }
      else if (name == "PIDfile")
      {
         pidfile = value;
      }
      else if (name == "UseX1tofPID")
      {
         useX1tofPID = get_bool(value);
      }
      else if (name == "X1tofPIDfile")
      {
         x1tofPIDfile = value;
      }
      else if (name == "UseEnergyGate")
      {
         useEnergyGate = get_bool(value);
      }
      else if (name == "EnergyMin")
      {
         energyMin = std::stod(value);
      }
      else if (name == "UseTDCGate")
      {
         useTDCGate = get_bool(value);
      }
      else if (name == "TDCGateLeft")
      {
         tdcGateLeft = std::stoi(value);
      }
      else if (name == "TDCGateRight")
      {
         tdcGateRight = std::stoi(value);
      }
      else if (name == "ExOxygenLeft")
      {
         exOxygenLeft = std::stod(value);
      }
      else if (name == "ExOxygenRight")
      {
         exOxygenRight = std::stod(value);
      }
      else if (name == "ExOxygenTop")
      {
         exOxygenTop = std::stod(value);
      }
      else if (name == "ExOxygenBottom")
      {
         exOxygenBottom = std::stod(value);
      }
      else if (name == "ExOxygenBackgroundLeft")
      {
         exOxygenBackgroundLeft = std::stod(value);
      }
      else if (name == "ExOxygenBackgroundRight")
      {
         exOxygenBackgroundRight = std::stod(value);
      }
      else if (name == "OxygenDoublePeakRatio")
      {
         oxygenDoublePeakRatio = std::stod(value);
      }
      else if (name == "ExCarbonLeft")
      {
         exCarbonLeft = std::stod(value);
      }
      else if (name == "ExCarbonRight")
      {
         exCarbonRight = std::stod(value);
      }
      else if (name == "ExCarbonTop")
      {
         exCarbonTop = std::stod(value);
      }
      else if (name == "ExCarbonBottom")
      {
         exCarbonBottom = std::stod(value);
      }
      else if (name == "ExCarbonBackgroundLeft")
      {
         exCarbonBackgroundLeft = std::stod(value);
      }
      else if (name == "ExCarbonBackgroundRight")
      {
         exCarbonBackgroundRight = std::stod(value);
      }
      else if (name == "PairEfficiency")
      {
         pairEfficiency = std::stod(value);
      }
   }
}

AnalysisConfig::~AnalysisConfig()
{
}

