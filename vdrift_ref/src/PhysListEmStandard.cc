// Arapuca simulation
// Authors: L. Paulucci & F. Marinho
// Date: 20th September 2016
//
// Added modifications should be reported in arapuca.cc header comments

#include "PhysListEmStandard.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4PhysicsListHelper.hh"

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4RayleighScattering.hh"
#include "G4KleinNishinaModel.hh"

#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

#include "G4hIonisation.hh"
#include "G4hBremsstrahlung.hh"
#include "G4hPairProduction.hh"

#include "G4ionIonisation.hh"
#include "G4IonParametrisedLossModel.hh"
#include "G4NuclearStopping.hh"

#include "G4LossTableManager.hh"
#include "G4UAtomicDeexcitation.hh"

#include "G4SystemOfUnits.hh"

PhysListEmStandard::PhysListEmStandard(const G4String& name)
   :  G4VPhysicsConstructor(name)
{}

PhysListEmStandard::~PhysListEmStandard()
{}

void PhysListEmStandard::ConstructProcess()
{
  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();
  
  // Add standard EM Processes
  //
  auto aParticleIterator=GetParticleIterator();
  aParticleIterator->reset();
  while( (*aParticleIterator)() ){
    G4ParticleDefinition* particle = aParticleIterator->value();
    G4String particleName = particle->GetParticleName();
     
    if (particleName == "gamma") {

      ////ph->RegisterProcess(new G4RayleighScattering, particle);      
      ph->RegisterProcess(new G4PhotoElectricEffect, particle);      
      G4ComptonScattering* cs   = new G4ComptonScattering;
      cs->SetEmModel(new G4KleinNishinaModel());
      ph->RegisterProcess(cs, particle);
      ph->RegisterProcess(new G4GammaConversion, particle);
     
    } else if (particleName == "e-") {

      G4eIonisation* eIoni = new G4eIonisation();
      eIoni->SetStepFunction(0.1, 100*um);      
      ph->RegisterProcess(eIoni,                   particle);
      ph->RegisterProcess(new G4eBremsstrahlung(), particle);      
            
    } else if (particleName == "e+") {

      G4eIonisation* eIoni = new G4eIonisation();
      eIoni->SetStepFunction(0.1, 100*um);      
      ph->RegisterProcess(eIoni,                     particle);
      ph->RegisterProcess(new G4eBremsstrahlung(),   particle);
      ph->RegisterProcess(new G4eplusAnnihilation(), particle);
                  
    } else if (particleName == "mu+" || 
               particleName == "mu-"    ) {

      G4MuIonisation* muIoni = new G4MuIonisation();
      muIoni->SetStepFunction(0.1, 50*um);      
      ph->RegisterProcess(muIoni,                   particle);
      ph->RegisterProcess(new G4MuBremsstrahlung(), particle);
      ph->RegisterProcess(new G4MuPairProduction(), particle);
                   
    } else if( particleName == "proton" ||
               particleName == "pi-" ||
               particleName == "pi+"    ) {
  
      G4hIonisation* hIoni = new G4hIonisation();
      hIoni->SetStepFunction(0.1, 20*um);
      ph->RegisterProcess(hIoni,                   particle);
      ph->RegisterProcess(new G4hBremsstrahlung(), particle);
      ph->RegisterProcess(new G4hPairProduction(), particle);            
     
    } else if( particleName == "alpha" || 
               particleName == "He3"    ) {

      G4ionIonisation* ionIoni = new G4ionIonisation();
      ionIoni->SetStepFunction(0.1, 1*um);
      ph->RegisterProcess(ionIoni,                 particle);
      ph->RegisterProcess(new G4NuclearStopping(), particle);      
            
    } else if( particleName == "GenericIon" ) {
 
      G4ionIonisation* ionIoni = new G4ionIonisation();
      ionIoni->SetEmModel(new G4IonParametrisedLossModel());
      ionIoni->SetStepFunction(0.1, 1*um);
      ph->RegisterProcess(ionIoni,                 particle);
      ph->RegisterProcess(new G4NuclearStopping(), particle);                   
      
    } else if ((!particle->IsShortLived()) &&
               (particle->GetPDGCharge() != 0.0) && 
               (particle->GetParticleName() != "chargedgeantino")) {
               
      //all others charged particles except geantino
      ph->RegisterProcess(new G4hIonisation(),    particle);
    }
  }
    
  // Deexcitation
  //
  G4VAtomDeexcitation* de = new G4UAtomicDeexcitation();
  de->SetFluo(true);
  de->SetAuger(false);   
  de->SetPIXE(false);  
  G4LossTableManager::Instance()->SetAtomDeexcitation(de);
}


