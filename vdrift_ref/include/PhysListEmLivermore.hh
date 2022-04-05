// Arapuca simulation
// Authors: L. Paulucci & F. Marinho
// Date: 20th September 2016
//
// Added modifications should be reported in arapuca.cc header comments

#ifndef PhysListEmLivermore_h
#define PhysListEmLivermore_h 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

class PhysListEmLivermore : public G4VPhysicsConstructor
{
public:
  PhysListEmLivermore(const G4String& name = "Livermore");
 ~PhysListEmLivermore();

  virtual void ConstructParticle() { };
  virtual void ConstructProcess();
};

#endif






