#pragma once

#include <G4VHit.hh>
#include <G4THitsCollection.hh>
#include <G4Allocator.hh>
#include <G4ThreeVector.hh>

namespace Test
{

class CaloHit : public G4VHit
{
public:
    CaloHit();
    ~CaloHit();

    // operators
    const CaloHit& operator=(const CaloHit& other);
    inline void* operator new(size_t);
    inline void operator delete(void*);
    
    G4bool CheckIDMatch(G4int caloID, G4int trackID);

    // functions
    void AddData(G4double edep);

    // override
    void Print() override;
    
    // setters
    void SetCaloID(G4int caloID) { fCaloID = caloID; }
    void SetTrackID(G4int trackID) { fTrackID = trackID; }

    // getters
    G4int GetCaloID() const { return fCaloID; }
    G4int GetTrackID() const { return fTrackID; }
    G4double GetEdep() const { return fEdep; }

private:
    G4int fCaloID;
    G4int fTrackID;
    G4double fEdep;
    // not here: pos and time
};

typedef G4THitsCollection<CaloHit> CaloHitsCollection;

extern G4ThreadLocal G4Allocator<CaloHit>* CaloHitAllocator;

inline void* CaloHit::operator new(size_t)
{
    if (!CaloHitAllocator) CaloHitAllocator = new G4Allocator<CaloHit>;
    return (void*)CaloHitAllocator->MallocSingle();
}

inline void CaloHit::operator delete(void* hit)
{
    CaloHitAllocator->FreeSingle((CaloHit*)hit);
}

}