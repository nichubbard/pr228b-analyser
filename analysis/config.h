#ifndef ANALYSIS_CONFIG_H
#define ANALYSIS_CONFIG_H

#include <string>
#include <vector>

class AnalysisConfig
{
   public:
      static AnalysisConfig& Instance();

      std::vector<int> Runs() const;
      bool UsePROOF() const;

      bool UsePID() const;
      std::string PIDfile() const;

      bool UseX1tofPID() const;
      std::string X1tofPIDfile() const;

      bool UseEnergyGate() const;
      double EnergyMin() const;

      bool UseTDCGate() const;
      int TDCGateLeft() const;
      int TDCGateRight() const;

      double ExOxygenLeft() const;
      double ExOxygenRight() const;
      double ExOxygenTop() const;
      double ExOxygenBottom() const;
      double ExOxygenBackgroundLeft() const;
      double ExOxygenBackgroundRight() const;
      double OxygenDoublePeakRatio() const;

      double ExCarbonLeft() const;
      double ExCarbonRight() const;
      double ExCarbonTop() const;
      double ExCarbonBottom() const;
      double ExCarbonBackgroundLeft() const;
      double ExCarbonBackgroundRight() const;

      double PairEfficiency() const;

   private:
      static AnalysisConfig* instance;

      AnalysisConfig();
      ~AnalysisConfig();

      AnalysisConfig(const AnalysisConfig& rhs);
      AnalysisConfig& operator=(const AnalysisConfig& rhs);

      std::vector<int> runs;
      bool usePROOF;
      bool usePID;
      std::string pidfile;
      bool useX1tofPID;
      std::string x1tofPIDfile;
      bool useEnergyGate;
      double energyMin;
      bool useTDCGate;
      int tdcGateLeft;
      int tdcGateRight;
      double exOxygenLeft;
      double exOxygenRight;
      double exOxygenTop;
      double exOxygenBottom;
      double exOxygenBackgroundLeft;
      double exOxygenBackgroundRight;
      double oxygenDoublePeakRatio;
      double exCarbonLeft;
      double exCarbonRight;
      double exCarbonTop;
      double exCarbonBottom;
      double exCarbonBackgroundLeft;
      double exCarbonBackgroundRight;
      double pairEfficiency;
};

inline AnalysisConfig::AnalysisConfig(AnalysisConfig const& rhs)
{
   instance = rhs.instance;
}

inline AnalysisConfig& AnalysisConfig::operator=(AnalysisConfig const& rhs)
{
   if (this != &rhs)
   {
      instance = rhs.instance;
   }
   return *this;
}

inline AnalysisConfig& AnalysisConfig::Instance()
{
   static AnalysisConfig theInstance;
   instance = &theInstance;
   return *instance;
}

inline std::vector<int> AnalysisConfig::Runs() const
{
   return runs;
}

inline bool AnalysisConfig::UsePROOF() const
{
   return usePROOF;
}

inline bool AnalysisConfig::UsePID() const
{
   return usePID;
}

inline std::string AnalysisConfig::PIDfile() const
{
   return pidfile;
}

inline bool AnalysisConfig::UseX1tofPID() const
{
   return useX1tofPID;
}

inline std::string AnalysisConfig::X1tofPIDfile() const
{
   return x1tofPIDfile;
}

inline bool AnalysisConfig::UseEnergyGate() const
{
   return useEnergyGate;
}

inline double AnalysisConfig::EnergyMin() const
{
   return energyMin;
}

inline bool AnalysisConfig::UseTDCGate() const
{
   return useTDCGate;
}

inline int AnalysisConfig::TDCGateLeft() const
{
   return tdcGateLeft;
}

inline int AnalysisConfig::TDCGateRight() const
{
   return tdcGateLeft;
}

inline double AnalysisConfig::ExOxygenLeft() const
{
   return exOxygenLeft;
}

inline double AnalysisConfig::ExOxygenRight() const
{
   return exOxygenRight;
}

inline double AnalysisConfig::ExOxygenTop() const
{
   return exOxygenTop;
}

inline double AnalysisConfig::ExOxygenBottom() const
{
   return exOxygenBottom;
}

inline double AnalysisConfig::ExOxygenBackgroundLeft() const
{
   return exOxygenBackgroundLeft;
}

inline double AnalysisConfig::ExOxygenBackgroundRight() const
{
   return exOxygenRight;
}

inline double AnalysisConfig::OxygenDoublePeakRatio() const
{
   return oxygenDoublePeakRatio;
}

inline double AnalysisConfig::ExCarbonLeft() const
{
   return exCarbonLeft;
}

inline double AnalysisConfig::ExCarbonRight() const
{
   return exCarbonRight;
}

inline double AnalysisConfig::ExCarbonTop() const
{
   return exCarbonTop;
}

inline double AnalysisConfig::ExCarbonBottom() const
{
   return exCarbonBottom;
}

inline double AnalysisConfig::ExCarbonBackgroundLeft() const
{
   return exCarbonBackgroundLeft;
}

inline double AnalysisConfig::ExCarbonBackgroundRight() const
{
   return exCarbonBackgroundRight;
}

inline double AnalysisConfig::PairEfficiency() const
{
   return pairEfficiency;
}

#endif /* ANALYSIS_CONFIG_H */

