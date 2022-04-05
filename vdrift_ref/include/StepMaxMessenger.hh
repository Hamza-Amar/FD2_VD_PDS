// Arapuca simulation
// Authors: L. Paulucci & F. Marinho
// Date: 20th September 2016
//
// Added modifications should be reported in arapuca.cc header comments

#ifndef StepMaxMessenger_h
#define StepMaxMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class StepMax;
class G4UIcmdWithADoubleAndUnit;

class StepMaxMessenger: public G4UImessenger
{
  public:
    StepMaxMessenger(StepMax*);
   ~StepMaxMessenger();
    
    virtual void SetNewValue(G4UIcommand*, G4String);
    
  private:
    StepMax*                   fStepMax;
    G4UIcmdWithADoubleAndUnit* fStepMaxCmd;
};

#endif
