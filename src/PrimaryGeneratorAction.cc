
#include "globals.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTypes.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
	: G4VUserPrimaryGeneratorAction(),
	  particleGun(0)
{

	G4int n_particle = 1;
	particleGun = new G4ParticleGun(n_particle);
	particleGun->SetParticleDefinition(G4Gamma::Gamma());
	particleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., -1.));
	particleGun->SetParticleEnergy(511.*keV);
	particleGun->SetParticlePosition(G4ThreeVector(0., 0., 20*1.125*cm + 1*mm));

}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{

	delete particleGun;

}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* Event)
{
	
	particleGun->GeneratePrimaryVertex(Event);

}
