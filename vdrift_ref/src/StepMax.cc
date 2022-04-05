// Arapuca simulation
// Authors: L. Paulucci & F. Marinho
// Date: 20th September 2016
//
// Added modifications should be reported in arapuca.cc header comments

#include "StepMax.hh"
#include "StepMaxMessenger.hh"

StepMax::StepMax(const G4String& processName)
 : G4VDiscreteProcess(processName),fMaxChargedStep(DBL_MAX),fMess(0)
{
  fMess = new StepMaxMessenger(this);
}

StepMax::~StepMax() { delete fMess; }

G4bool StepMax::IsApplicable(const G4ParticleDefinition& particle) 
{ 
  return (particle.GetPDGCharge() != 0.);
}
    
void StepMax::SetMaxStep(G4double step) {fMaxChargedStep = step;}

G4double StepMax::PostStepGetPhysicalInteractionLength(const G4Track&,
                                                  G4double,
                                                  G4ForceCondition* condition )
{
  // condition is set to "Not Forced"
  *condition = NotForced;
  
  return fMaxChargedStep;
}

G4VParticleChange* StepMax::PostStepDoIt(const G4Track& aTrack, const G4Step&)
{
   aParticleChange.Initialize(aTrack);
   return &aParticleChange;
}



