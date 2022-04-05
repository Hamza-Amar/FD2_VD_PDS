// Arapuca simulation
// Authors: L. Paulucci & F. Marinho
// Date: 20th September 2016
//
// Added modifications should be reported in arapuca.cc header comments

#ifndef PhysListEmPenelope_h
#define PhysListEmPenelope_h 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

class PhysListEmPenelope : public G4VPhysicsConstructor
{
public:
  PhysListEmPenelope(const G4String& name = "Penelope");
 ~PhysListEmPenelope();

  virtual void ConstructParticle() { };
  virtual void ConstructProcess();
};

#endif






