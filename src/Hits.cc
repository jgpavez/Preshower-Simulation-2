#include "Hits.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4Allocator<Hits> HitAllocator;

Hits::Hits()
	: G4VHit(),
	  energy(0),
	  pos(G4ThreeVector())
{}

Hits::Hits(G4double Energy, G4ThreeVector Pos){

	energy	= Energy;
	pos = Pos;
}

Hits::~Hits()
{}

void Hits::Draw(){

	G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
	if(pVVisManager){
		G4Circle circle(pos);
		circle.SetScreenSize(2.);
		circle.SetFillStyle(G4Circle::filled);
		G4Colour colour(1.,1.,0.);
		G4VisAttributes attribs(colour);
		circle.SetVisAttributes(attribs);
		pVVisManager->Draw(circle);
  	}

}

void Hits::Print()
{
	G4cout<<"Energy: "<<std::setw(7) << G4BestUnit(energy,"Energy")<<G4endl;
}
