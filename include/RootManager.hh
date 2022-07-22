#pragma once

#include <TString.h>
#include "globals.hh"
#include <TClonesArray.h>

#include <G4ThreadLocalSingleton.hh>

class TFile;
class TTree;

class CaloData;
class SipmData;

namespace Test
{

class RootManager {
friend class G4ThreadLocalSingleton<RootManager>;
public:
    static RootManager* GlobalInstance();
    static RootManager* Instance();
    ~RootManager();

    // operation
    bool Open();
    Int_t Fill();
    bool WriteAndClose();

    // getters
    TString GetFileName() { return fFileName; };
    
    CaloData* GetNewCalo();
    SipmData* GetNewSipm();

    // setters
    void SetFileName(TString newName) { if (!fIsOpen) fFileName = newName; };

private:
    RootManager();

    void InitializeFromGlobal();

    // internal
    static RootManager* fTheGlobalInstance;
    bool fIsOpen;
    TFile* fTheFile;
    TTree* fTheTree;

    // config
    TString fFileName;

    // data fields
    TClonesArray* fCaloVec;
    TClonesArray* fSipmVec;
};

}