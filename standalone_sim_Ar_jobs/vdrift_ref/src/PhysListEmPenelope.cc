// Arapuca simulation
// Authors: L. Paulucci & F. Marinho
// Date: 20th September 2016
//
// Added modifications should be reported in arapuca.cc header comments

#include "PhysListEmPenelope.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"

// gamma

#include "G4PhotoElectricEffect.hh"
#include "G4PenelopePhotoElectricModel.hh"

#include "G4ComptonScattering.hh"
#include "G4PenelopeComptonModel.hh"

#include "G4GammaConversion.hh"
#include "G4PenelopeGammaConversionModel.hh"

#include "G4RayleighScattering.hh" 
#include "G4PenelopeRayleighModel.hh"

// e-

#include "G4eIonisation.hh"
#include "G4PenelopeIonisationModel.hh"
#include "G4UniversalFluctuation.hh"

#include "G4eBremsstrahlung.hh"
#include "G4PenelopeBremsstrahlungModel.hh"

// e+

#include "G4eplusAnnihilation.hh"
#include "G4PenelopeAnnihilationModel.hh"

// mu

#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

// hadrons, ions

#include "G4hIonisation.hh"
#include "G4ionIonisation.hh"

#include "G4SystemOfUnits.hh"

PhysListEmPenelope::PhysListEmPenelope(const G4String& name)
  :  G4VPhysicsConstructor(name)
{ }

PhysListEmPenelope::~PhysListEmPenelope()
{ }

void PhysListEmPenelope::ConstructProcess()
{
  // Add standard EM Processes
  auto aParticleIterator=GetParticleIterator();
  aParticleIterator->reset();
  while( (*aParticleIterator)() ){
    G4ParticleDefinition* particle = aParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();

    //Applicability range for Penelope models
    //for higher energies, the Standard models are used   
    G4double highEnergyLimit = 1*GeV;
         
    if (particleName == "gamma") {
      // gamma         

      G4PhotoElectricEffect* phot = new G4PhotoElectricEffect();
      G4PenelopePhotoElectricModel* 
      photModel = new G4PenelopePhotoElectricModel();
      photModel->SetHighEnergyLimit(highEnergyLimit);
      phot->AddEmModel(0, photModel);
      pmanager->AddDiscreteProcess(phot);

      G4ComptonScattering* compt = new G4ComptonScattering();
      G4PenelopeComptonModel* 
      comptModel = new G4PenelopeComptonModel();
      comptModel->SetHighEnergyLimit(highEnergyLimit);
      compt->AddEmModel(0, comptModel);
      pmanager->AddDiscreteProcess(compt);

      G4GammaConversion* conv = new G4GammaConversion();
      G4PenelopeGammaConversionModel* 
      convModel = new G4PenelopeGammaConversionModel();
      convModel->SetHighEnergyLimit(highEnergyLimit);
      conv->AddEmModel(0, convModel);
      pmanager->AddDiscreteProcess(conv);

      G4RayleighScattering* rayl = new G4RayleighScattering();
      G4PenelopeRayleighModel* 
      raylModel = new G4PenelopeRayleighModel();
      raylModel->SetHighEnergyLimit(highEnergyLimit);
      rayl->AddEmModel(0, raylModel);
      pmanager->AddDiscreteProcess(rayl);
      
    } else if (particleName == "e-") {
      //electron

      G4eIonisation* eIoni = new G4eIonisation();
      G4PenelopeIonisationModel* 
      eIoniModel = new G4PenelopeIonisationModel();
      eIoniModel->SetHighEnergyLimit(highEnergyLimit); 
      eIoni->AddEmModel(0, eIoniModel, new G4UniversalFluctuation() );
      pmanager->AddProcess(eIoni,                   -1, 1, 1);
      
      G4eBremsstrahlung* eBrem = new G4eBremsstrahlung();
      G4PenelopeBremsstrahlungModel* 
      eBremModel = new G4PenelopeBremsstrahlungModel();
      eBremModel->SetHighEnergyLimit(highEnergyLimit);
      eBrem->AddEmModel(0, eBremModel);
      pmanager->AddProcess(eBrem,                   -1, 2, 2);
                  
    } else if (particleName == "e+") {
      //positron
      G4eIonisation* eIoni = new G4eIonisation();
      G4PenelopeIonisationModel* 
      eIoniModel = new G4PenelopeIonisationModel();
      eIoniModel->SetHighEnergyLimit(highEnergyLimit); 
      eIoni->AddEmModel(0, eIoniModel, new G4UniversalFluctuation() );
      pmanager->AddProcess(eIoni,                   -1, 1, 1);
      
      G4eBremsstrahlung* eBrem = new G4eBremsstrahlung();
      G4PenelopeBremsstrahlungModel* 
      eBremModel = new G4PenelopeBremsstrahlungModel();
      eBremModel->SetHighEnergyLimit(highEnergyLimit);
      eBrem->AddEmModel(0, eBremModel);
      pmanager->AddProcess(eBrem,                   -1, 2, 2);      

      G4eplusAnnihilation* eAnni = new G4eplusAnnihilation();
      G4PenelopeAnnihilationModel* 
      eAnniModel = new G4PenelopeAnnihilationModel();
      eAnniModel->SetHighEnergyLimit(highEnergyLimit); 
      eAnni->AddEmModel(0, eAnniModel);
      pmanager->AddProcess(eAnni,                    0,-1, 3);
            
    } else if( particleName == "mu+" || 
               particleName == "mu-"    ) {
      //muon  
      pmanager->AddProcess(new G4MuIonisation,      -1, 1, 1);
      pmanager->AddProcess(new G4MuBremsstrahlung,  -1, 2, 2);
      pmanager->AddProcess(new G4MuPairProduction,  -1, 3, 3);       
     
    } else if( particleName == "alpha" || particleName == "GenericIon" ) { 
      pmanager->AddProcess(new G4ionIonisation,     -1, 1, 1);

    } else if ((!particle->IsShortLived()) &&
               (particle->GetPDGCharge() != 0.0) && 
               (particle->GetParticleName() != "chargedgeantino")) {
      //all others charged particles except geantino
      pmanager->AddProcess(new G4hIonisation,       -1, 1, 1);
    }
  }
}


