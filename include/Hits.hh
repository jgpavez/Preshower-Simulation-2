#ifndef Hits_h
#define Hits_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class Hits : public G4VHit{

public:
	Hits();
	Hits(G4double, G4ThreeVector);
	~Hits();
	
	inline void* operator new(size_t);
	inline void  operator delete(void*);

	void Draw();
	void Print();	

	void setEnergy(G4double Energy)	{energy = Energy;};
	G4double getEnergy() const	{return energy;  };
	void setPos(G4ThreeVector Pos)	{pos = Pos;};
	G4ThreeVector getPos() const	{return pos;  };

private:
	G4double energy;
	G4ThreeVector pos;
};

typedef G4THitsCollection<Hits> HitsCollection;
extern G4Allocator<Hits> HitAllocator;

inline void* Hits::operator new(size_t){

	void* hit;
	hit = (void*)HitAllocator.MallocSingle();
	return hit;
}

inline void Hits::operator delete(void* hit){

	HitAllocator.FreeSingle((Hits*)hit);
}
#endif
