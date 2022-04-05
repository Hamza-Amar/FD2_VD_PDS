// Arapuca simulation
// Authors: L. Paulucci & F. Marinho
// Date: 20th September 2016
//
// Added modifications should be reported in arapuca.cc header comments

#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4ThreeVector.hh"
#include "TH1D.h"

class G4Event;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  PrimaryGeneratorAction();    
  PrimaryGeneratorAction(double x, double y, double z);    
  ~PrimaryGeneratorAction();

  void GeneratePrimaries(G4Event*);
  G4ThreeVector Polarisation(G4ThreeVector d);
  G4ThreeVector TransversePosition(G4ThreeVector d, double r);
  void DepthSampling();
  void GetEMShowerParameters();
  
private:
  G4ParticleGun*           fParticleGun;
  double x0;
  double y0;
  double z0;
  TH1D* hd;
  double de, p, rc, rt;

  double d0,d1,d2,d3;
  double p0,p1,p2,p3,p4;
  double c0,c1,c2,c3;
  double t0,t1,t2,t3;
};

#endif


