#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "Randomize.hh"
#include "PrimaryGeneratorAction.hh"
#include "TF1.h"
#include "TMath.h"
#include "TFormula.h"
#include "TH1D.h"
#include "TRandom3.h"

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  fParticleGun  = new G4ParticleGun(1);

}

PrimaryGeneratorAction::PrimaryGeneratorAction(double x, double y, double z)
{
  fParticleGun  = new G4ParticleGun(1);
  x0 = x;
  y0 = y;
  z0 = z;

  /*x0 = CLHEP::RandFlat::shoot(0.0,6.75);
  y0 = CLHEP::RandFlat::shoot(-3.375,3.375);
  z0 = CLHEP::RandFlat::shoot(0.0,0.75);*/

  hd = new TH1D("hd","",40,0.0,400.0);

  //d0 = 3.36251e+00; //0.5GeV //4.12583e+00; //0.3GeV
  //d1 = 1.04612e+00;          //7.93660e-01;
  //d2 = 1.32043e-01;          //1.68173e-01;
  //d3 = 1.46733e+03;          //7.13234e+02;

  //d0 = 2.90781e+01;  //6GeV //0.65m max
  //d1 = 2.47529e+00;
  //d2 = 1.15690e+00;
  //d3 = 2.04561e+03;
  
  //d0 = 1.02162e+01; //2GeV 
  //d1 = 1.79657e+00;        
  //d2 = 3.99390e-01;        
  //d3 = 1.93963e+03;        
    
  //d0 = 1.19566e+01; //1GeV //5.98928e+01; //3GeV //1.05058e+01; //7GeV
  //d1 = 1.40577e+00;        //2.03621e+00;        //2.54499e+00;
  //d2 = 4.67183e-01;        //2.35022e+00;        //4.16829e-01;
  //d3 = 8.28395e+02;        //4.95948e+02;        //6.61074e+03;
    
  /*double del = hd->GetBinWidth(1);
  double dp;
  for(int i=1;i<=40;i++){
    dp = (i-0.5)*del;
    hd->SetBinContent(i,d3*pow(dp/d0,d1)*(pow(d2,d1+1.0)/TMath::Gamma(d1+1.0))*exp(-d2*dp/d0));
    }*/

  //for(int i=1;i<=40;i++)std::cout << hd->GetBinContent(i) << std::endl;
  
  //7GeV
  /*p0 = 1.34439;
  p1 = 135.195;
  p2 = 63.4978;
  p3 = 0.860582;
  p4 = 0.0255285;

  c0 = 5.07772;
  c1 = -0.0912049;
  c2 = 0.000365051;
  c3 = -5.26577e-07;

  t0 = 0.461543;
  t1 = -0.0492471;
  t2 = 8.37309e-05;
  t3 = 5.15859e-08;*/

  //6GeV
  /*p0 = 1.40126;
  p1 = 129.177;
  p2 = 58.6178;
  p3 = 0.913409;
  p4 = 0.0227795;

  c0 = 3.18876;
  c1 = -0.0563035;
  c2 = 0.000155281;
  c3 = -1.12692e-07;

  t0 = 1.52865;
  t1 = -0.0842441;
  t2 = 0.000333252;
  t3 = -4.3255e-07;*/
  
  //3GeV
  /*p0 = 1.45623;
  p1 = 108.865;
  p2 = 46.5248;
  p3 = 0.917771;
  p4 = 0.0225988;

  c0 = 3.33049;
  c1 = -0.0859974;
  c2 = 0.000427221;
  c3 = -6.90903e-07;

  t0 = 0.231023;
  t1 = -0.0718456;
  t2 = 0.000312326;
  t3 = -4.62997e-07;*/

  //2GeV
  /*p0 = 1.4358;
  p1 = 107.875;
  p2 = 47.6053;
  p3 = 0.938062;
  p4 = 0.0242635;

  c0 = 3.07784;
  c1 = -0.0854082;
  c2 = 0.000446278;
  c3 = -9.26292e-07;

  t0 = 0.401962;
  t1 = -0.0832622;
  t2 = 0.000425091;
  t3 =-7.49333e-07;*/

  //1GeV
  /*p0 = 1.33857;
  p1 = 80.2784;
  p2 = 32.6105;
  p3 = 0.95563;
  p4 = 0.0294062;

  c0 = 2.84961;
  c1 = -0.101501;
  c2 = 0.000642222;
  c3 = -1.47131e-06;

  t0 = 0.0642751;
  t1 = -0.0705169;
  t2 = 0.000211356;
  t3 = 2.0126e-07;*/
  
  //0.3GeV
  //p0 = -0.014409;
  //p1 = 0.0864855;
  //p2 = 0.000117135;

  //0.5GeV
  //p0 = 0.253673;
  //p1 = 0.0549595;
  //p2 = 0.000146144;
  
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  G4int i=0;
  G4double theta,phi;
  G4double test;
    
  phi = CLHEP::twopi*CLHEP::RandFlat::shoot(0.0,1.0)*rad; 

  while(i==0){
     theta = CLHEP::RandFlat::shoot(0.0,CLHEP::pi)*rad;
     test = CLHEP::RandFlat::shoot(0.0,1.0);
     if(sin(theta)>test)i++;
  }

  //Particle direction
  G4double kx, ky, kz; //components
  
  kx=cos(phi)*sin(theta);
  ky=sin(phi)*sin(theta);
  kz=cos(theta);
  G4ThreeVector dir_vec (kx,ky,kz);

  //Polarization 
  G4ThreeVector polar = Polarisation(dir_vec);
  
  fParticleGun->SetParticleEnergy(9.68*eV);
//  fParticleGun->SetParticleEnergy(7.08*eV);
  fParticleGun->SetParticleMomentumDirection(dir_vec);
  fParticleGun->SetParticlePosition(G4ThreeVector(x0*m,y0*m,z0*m));
  //fParticleGun->SetParticlePosition(G4ThreeVector(x0*cm,y0*cm,z0*cm));
  fParticleGun->SetParticlePolarization(polar); 
  
  G4ParticleDefinition* particle=
    //G4ParticleTable::GetParticleTable()->FindParticle("mu-");
    G4ParticleTable::GetParticleTable()->FindParticle("opticalphoton");
  
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->GeneratePrimaryVertex(anEvent);

}

G4ThreeVector PrimaryGeneratorAction::Polarisation(G4ThreeVector d){

  if(d.mag()!=1.0) d = d.unit();

  G4ThreeVector a = d.orthogonal();
  G4ThreeVector b = a.cross(d); 
  a = a.unit();
  b = b.unit();
  G4double theta = CLHEP::twopi*G4UniformRand();
  return cos(theta)*a + sin(theta)*b;

}

G4ThreeVector PrimaryGeneratorAction::TransversePosition(G4ThreeVector d, double r){

  if(d.mag()!=1.0) d = d.unit();

  G4ThreeVector a = d.orthogonal();
  G4ThreeVector b = a.cross(d); 
  a = a.unit();
  b = b.unit();
  G4double theta = CLHEP::twopi*G4UniformRand();
  return r*(cos(theta)*a + sin(theta)*b);

}


void PrimaryGeneratorAction::DepthSampling(){


  

}

void PrimaryGeneratorAction::GetEMShowerParameters(){

  
}
