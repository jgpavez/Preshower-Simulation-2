
#include "SensitiveDetector.hh"
#include "Hits.hh"
#include "RunAction.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4OpticalPhoton.hh"
#include "G4SDManager.hh"
#include "G4StepPoint.hh"
#include "G4ios.hh"
#include "Analysis.hh"


SensitiveDetector::SensitiveDetector(const G4String& name, const G4String& hitsCName) 
	: G4VSensitiveDetector(name),
	  point(0),
	  hitsCollection(0),
	  slice(0),
	  sliceY(0),
	  sliceX(0),
	  energy(0),
	  pos(G4ThreeVector())
{
	  collectionName.insert(hitsCName);
}

SensitiveDetector::~SensitiveDetector()
{}

void SensitiveDetector::Initialize(G4HCofThisEvent*)
{	
	
}
G4bool SensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*){ 

//	G4RunManager* runManager = G4RunManager::GetRunManager();
//	RunAction* runAction = (RunAction*)runManager->GetUserRunAction();

	//slice = step->GetPreStepPoint()->GetTouchableHandle()->GetReplicaNumber();
	slice = 0;
	energy = 0.;
	//slice = step->GetPreStepPoint()->GetTouchableHandle()->GetReplicaNumber(0);
	sliceY = step->GetPreStepPoint()->GetTouchableHandle()->GetReplicaNumber(1);
	sliceX = step->GetPreStepPoint()->GetTouchableHandle()->GetReplicaNumber(0);
	if ( step->GetTrack()->GetDefinition() == G4OpticalPhoton::OpticalPhoton()){
		//energy = step->GetTotalEnergyDeposit();
		energy = 1.;
	}


	energyArray[sliceX][sliceY] += energy;
	//pos = step->GetPreStepPoint()->GetPosition();
	//if (pos.getZ() > (225.*mm - 45*mm*0/4) && pos.getZ() <  (225.*mm - 45*mm*1/4) ) slice = 0;
	//if (pos.getZ() > (225.*mm - 45*mm*1/4) && pos.getZ() <  (225.*mm - 45*mm*2/4) ) slice = 1;
	//if (pos.getZ() > (225.*mm - 45*mm*2/4) && pos.getZ() <  (225.*mm - 45*mm*3/4) ) slice = 2;
	//if (pos.getZ() > (225.*mm - 45*mm*3/4) && pos.getZ() <  (225.*mm - 45*mm*4/4) ) slice = 3;
	return true;

}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
	G4RunManager* runManager = G4RunManager::GetRunManager();
	RunAction* runAction = (RunAction*)runManager->GetUserRunAction();

	energy = 0.;
	for (int i = 0; i < 25; i++){
		for (int j = 0; j < 25; j++){
			runAction->tuple->Fill(energyArray[i][j], i, j);
			energyArray[i][j] = 0.;
		}
	}

}

