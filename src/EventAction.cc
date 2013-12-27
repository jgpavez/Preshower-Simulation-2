#include "EventAction.hh"
#include "G4Event.hh"

EventAction::EventAction()
{}

EventAction::~EventAction()
{}

void EventAction::BeginOfEventAction(const G4Event* event){

	G4cout<<"Event "<<event->GetEventID()<<" start."<<G4endl;
}

void EventAction::EndOfEventAction(const G4Event* event){

	G4cout<<"Event "<<event->GetEventID()<<" done."<<G4endl;
}
