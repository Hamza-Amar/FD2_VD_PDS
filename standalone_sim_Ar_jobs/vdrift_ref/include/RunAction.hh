// Arapuca simulation
// Authors: L. Paulucci & F. Marinho
// Date: 20th September 2016
//
// Added modifications should be reported in arapuca.cc header comments

#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"

#include "DetectorConstruction.hh"

class G4Run;

class RunAction : public G4UserRunAction
{
public:
  
  RunAction(DetectorConstruction*);
  ~RunAction();

  void BeginOfRunAction(const G4Run*);
  void EndOfRunAction(const G4Run*);
    
  void  SetRndmFreq(G4int   val)  {fSaveRndm = val;}
  G4int GetRndmFreq()             {return fSaveRndm;}

  G4int GetNumEvent(){return fNumEvent;}
  void SetNumEvent(G4int i){fNumEvent = i;}

private:

  DetectorConstruction* fDetector;    

  G4int fSaveRndm;
  G4int fNumEvent;

};

#endif
