// Arapuca simulation
// Authors: L. Paulucci & F. Marinho
// Date: 20th September 2016
//
// Added modifications should be reported in arapuca.cc header comments

#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"

#include "RunAction.hh"
#include "DetectorConstruction.hh"
#include <string>

class SteppingAction : public G4UserSteppingAction
{
public:
  SteppingAction(RunAction* ,DetectorConstruction*);
  ~SteppingAction();
  
  void UserSteppingAction(const G4Step*);
  std::pair<int,int> VolumeCode(std::string name); 
  
private:
  RunAction*            fRun;
  DetectorConstruction* fDetector;
  std::map<std::string, int> imap;
  std::map<std::string, int>::iterator p;
  int idx;
  int eveti;
};

#endif




