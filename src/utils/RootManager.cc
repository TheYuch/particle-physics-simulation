#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
#endif
#include <TFile.h>
#include <TTree.h>
#include <TROOT.h>
#ifdef __clang__
#pragma clang diagnostic pop
#endif

#include "utils/RootManager.hh"

#include <G4Run.hh>
#include <G4RunManager.hh>
#include <G4AutoLock.hh>

namespace {
    G4Mutex mutexFileIO = G4MUTEX_INITIALIZER;
}

RootManager::RootManager()
: fIsOpen(false)
, fTheFile(nullptr)
, fTheTree(nullptr)
, fFileName(Constants::DefaultFileName)
{
    if (G4Threading::G4GetThreadId() != -1) {
        // This instance is created in a worker thread
        // Copy all fields from the configured global instance
        InitializeFromGlobal();
    }

    // TODO: do this step with a data file (like JSON or GDML)
    fRootDataVecs[Constants::RootDataType::Calo] = new TClonesArray("CaloRootData", 5);
    fRootDataVecs[Constants::RootDataType::Sipm] = new TClonesArray("SipmRootData", 10);
}

RootManager::~RootManager()
{
    if (fIsOpen) {
        WriteAndClose();
    }
    
    for (auto const& rootDataVecPair : fRootDataVecs)
    {
        rootDataVecPair.second->Delete();
        delete rootDataVecPair.second;
    }
}

RootManager* RootManager::fTheGlobalInstance = nullptr;
RootManager* RootManager::GlobalInstance()
{
    if (!fTheGlobalInstance)
    {
        fTheGlobalInstance = new RootManager;
    }
    return fTheGlobalInstance;
}

RootManager* RootManager::Instance()
{
    static G4ThreadLocalSingleton<RootManager> instance;
    if (G4Threading::G4GetThreadId() == -1)
    {
        return GlobalInstance();
    }
    return instance.Instance();
}

void RootManager::InitializeFromGlobal()
{
    RootManager* gInst = GlobalInstance();

    fFileName = gInst->GetFileName();
}

bool RootManager::Open()
{
    if (fIsOpen)
    {
        G4cerr << "File is already open. Write and close it first." << G4endl;
        return false;
    }
    if (fFileName == "")
    {
        fFileName = Constants::DefaultFileName;
        G4cerr << "No file name given for ROOT ouput. Initializing it to defalut: " << fFileName << G4endl;
    }

    G4int threadID = G4Threading::G4GetThreadId();
    if (threadID == -1) // if this is the master thread
    {
        return false;
    }

    TString runString = TString::Format("%05d", G4RunManager::GetRunManager()->GetCurrentRun()->GetRunID());
    TString threadString = TString::Format("%02d", threadID);
    TString name = fFileName;

    // Replace each {RUN} with the actual run number
    Int_t pos = 0;
    while ( (pos = name.First("#RUN#")) > 0) {
        name.Replace(pos, 5, runString, runString.Sizeof());
    }

    // Append thread id and file extension
    name += "-" + threadString + ".root";

    // Mutex Lock. Only one file interaction to be done at a time.
    G4AutoLock aLock(&mutexFileIO);
    fTheFile = TFile::Open(name, "RECREATE");
    if (!fTheFile)
    {
        G4cerr << "File " << name << " could not be created." << G4endl;
        fIsOpen = false;
        return fIsOpen;
    }


    // With the file open, start generating objects
    fTheTree = new TTree("sim", "sim");
    for (auto const& rootDataVecPair : fRootDataVecs)
    {
        fTheTree->Branch(Constants::RootDataTypeNames.at(rootDataVecPair.first), "TClonesArray", rootDataVecPair.second, 32000, 99);
    }
    

    // Return
    fIsOpen = true;
    return fIsOpen;
}

Int_t RootManager::Fill()
{
    Int_t nBytes = -2;
    if (fIsOpen && fTheTree) {
        nBytes = fTheTree->Fill();
    } else {
        G4cerr << "Warning: Could not fill TTree as either the file or the tree is unavailable" << G4endl;
    }

    for (auto const& rootDataVecPair : fRootDataVecs)
    {
        rootDataVecPair.second->Clear("C");
    }
    
    return nBytes;
}

bool RootManager::WriteAndClose()
{
    if (!fIsOpen)
    {
        if (G4Threading::G4GetThreadId() != -1) // master thread does not write to a root file, so it's error is ignored.
        {
            G4cerr << "File is not open, cannot write and close." << G4endl;
        }
        return false;
    }

    // Mutex Lock. Only one file interaction to be done at a time.
    G4AutoLock aLock(&mutexFileIO);
    fTheFile->cd();
    
    if (fTheTree) fTheTree->Write();

    fTheFile->Close();
    delete fTheFile;
    fTheTree = nullptr;
    fTheFile = nullptr;

    fIsOpen = false;

    return true;
}

TObject* RootManager::GetNewRootData(Constants::RootDataType rootDataType)
{
    TClonesArray* rootDataVec = fRootDataVecs[rootDataType];
    return rootDataVec->ConstructedAt(rootDataVec->GetEntries()); // for example, fCaloVec->GetEntries() is index of element past the last one, which the function gets, maybe allocates, and returns
}