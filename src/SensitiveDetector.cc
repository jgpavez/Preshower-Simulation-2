
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

	G4TouchableHandle tHandle = step->GetPreStepPoint()->GetTouchableHandle();
	G4String rName = tHandle->GetVolume()->GetName();
	G4int slice = tHandle->GetReplicaNumber();
	G4int axis;
	if (rName == "MPPC_X") axis = 0;
	else{
 		if (rName == "MPPC_Y") axis = 1;
		else{
			axis = -1;
			G4cout << "******** Readout error ********** " << G4endl;
		}
	}
	energy = 0.;
	if ( step->GetTrack()->GetDefinition() == G4OpticalPhoton::OpticalPhoton()){
		//energy = step->GetTotalEnergyDeposit();
		energy = 1.;
	}

	if (axis >= 0)
		energyArray[axis][slice] += energy;
	return true;

}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
	G4RunManager* runManager = G4RunManager::GetRunManager();
	RunAction* runAction = (RunAction*)runManager->GetUserRunAction();

	energy = 0.;
	for (int i = 0; i < 2; i++){
		for (int j = 0; j < 25; j++){
			runAction->tuple->Fill(energyArray[i][j], i, j);
			energyArray[i][j] = 0.;
		}
	}
}

