// Arapuca simulation
// Authors: L. Paulucci & F. Marinho
// Date: 20th September 2016
//
// Added modifications should be reported in arapuca.cc header comments

#ifndef PhysListEmStandard_h
#define PhysListEmStandard_h 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

class PhysListEmStandard : public G4VPhysicsConstructor
{
public: 
  PhysListEmStandard(const G4String& name = "standard");
 ~PhysListEmStandard();

public: 
  virtual void ConstructParticle() {};
 
  virtual void ConstructProcess();
};

#endif








